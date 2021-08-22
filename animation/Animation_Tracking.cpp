#include "core/Common.h"
#include "OpForwardDeclare.h"
#include "AnimationData.h"
#include "event/Events.h"
#include "Animation_Base.h"
#include "Animation_Tracking.h"
#include "util/Rotations.h"
#include "util/Calc.h"

Animation_Tracking::Animation_Tracking(ANIMATIONDATA *_data) : Animation_Base(_data)
{
}


Animation_Tracking::~Animation_Tracking()
{
}


void Animation_Tracking::initFacing()
{
	facing = data->facing;
}

// the tracking animation needs quite a bit of a different aproach, because technically, it's two animations, and they behave a bit differently
//from the usual and require additional data.
void Animation_Tracking::AddAnimationToVessel(VESSEL4 *_vessel)
{
	initFacing();
	vessel = _vessel;
	primaryaxis = mul(data->rotationMatrix, data->components[0]->axis);
	
	//create primary animation on vessel and remember the orbiter ID 
	orbiterid = createAnim(0.0);
	//create the component for the primary animation
	animationcomponents.push_back(vessel->AddAnimationComponent(orbiterid, data->components[0]->duration[0], data->components[0]->duration[1],
		createRotationComponent(data->components[0], data->rotationMatrix)));

	//check if there is a secondary animation and create if yes
	if (data->components.size() > 1)
	{
		secondaryorbiterid = createAnim(0.0);
		secondaryaxis = mul(data->rotationMatrix, data->components[1]->axis);
	}

	Olog::debug("created tracking animation %s %i", data->id.data(), orbiterid);

	//if there's a secondary animation, create its component too
	if (secondaryorbiterid != -1)
	{
		secondrotation = createRotationComponent(data->components[1], data->rotationMatrix);
		animationcomponents.push_back(vessel->AddAnimationComponent(secondaryorbiterid, data->components[1]->duration[0], data->components[1]->duration[1],
			secondrotation));
	}
	
	//transform the direction the animation needs to face
	facing = mul(data->rotationMatrix, facing);
	
	//if the animation is tracking a target or retreating when it is created, immediately calculate the target states and the shortest way
	if (target != NULL || stopanimation)
	{
		calculateTargetState();
		//if the animation is already running, calculate the direction it has to be moving in
		if (speed != 0)
		{
			speed = abs(speed) * determineMovementDirection(primarytargetstate, state, data->components[0]->range >= 360);
			secondaryspeed = abs(secondaryspeed) * determineMovementDirection(secondarytargetstate, secondarystate, data->components[1]->range >= 360);
		}
	}

	EnableAnimation();
	//if the animation is on the move when being created, we have to set the axis of the secondary animation
	if (state != primarytargetstate || secondarystate != secondarytargetstate)
	{
		initMovingAnimation();
	}

}


void Animation_Tracking::RemoveAnimationFromVessel()
{
	Olog::debug("Deleting animation %s %i", data->id.data(), orbiterid);

	DisableAnimation();
	//delete the animationcomponents in reverse order.
	//technically DelAnimation should do that, but it 
	//seems it doesn't get the order right and crashes.
	for (UINT i = animationcomponents.size(); i > 0; --i)
	{
		vessel->DelAnimationComponent(orbiterid, animationcomponents[i - 1]);
	}
	animationcomponents.clear();

	//delete the secondary animation
	bool result = vessel->DelAnimation(secondaryorbiterid);
	secondaryorbiterid = -1;
	
	result = vessel->DelAnimation(orbiterid);
	orbiterid = -1;
	
	enabled = false;

	//delete the meshgroup identifiers
	for (UINT i = 0; i < animationgroups.size(); ++i)
	{
		delete[] animationgroups[i];
	}
	animationgroups.clear();

}


AnimationEvent_Base *Animation_Tracking::PropagateAnimation(double simdt)
{
	AnimationEvent_Base *statusevent = NULL;
	if (speed != 0.0)
	{
		if (enabled && initmovinganim)
		{
			calculateTargetState();
			initmovinganim = false;
		}
		if (primarytargetstate <= 1.0 && 
			propagateState(state, primarytargetstate, speed, simdt, data->components[0]->range >= 360))
		{
			//the first state is aligned, propagate the second one
			if (secondarytargetstate <= 1.0 &&
				propagateState(secondarystate, secondarytargetstate, secondaryspeed, simdt, data->components[1]->range >= 360))
			{
				//the second one is aligned too. Depending on whether the animation is stoping or tracking, this means different things.
				if (stopanimation)
				{
					//the animation was reverting back to origin, so we can now stop and reset it
					speed = 0.0;
					secondaryspeed = 0.0;
					stopanimation = false;
					statusevent = new AnimationFinishedEvent(data->id);
				}
				else
				{
					//otherwise, the animation is now aligned
					statusevent = new TrackingAnimationStatusEvent(data->id, target, ALIGNED);
				}
			}
			else if (secondarytargetstate > 1.0)
			{
				//the required state cannot be reached with the current vessel alignement
				statusevent = new TrackingAnimationStatusEvent(data->id, target, UNABLE);
			}
		}
		else if (primarytargetstate > 1.0)
		{
			//the required state cannot be reached with the current vessel alignement
			statusevent = new TrackingAnimationStatusEvent(data->id, target, UNABLE);
		}

		if (statusevent == NULL)
		{
			//the animation is either aligning or reverting, since it isn't aligned or finished
			if (stopanimation)
			{
				//if the animation is in the process of stoping, it is currently reverting to origin
				statusevent = new TrackingAnimationStatusEvent(data->id, target, REVERTING);
			}
			else
			{
				//otherwise, it's aligning with a target
				statusevent = new TrackingAnimationStatusEvent(data->id, target, ALIGNING);
			}
		}
		else
		{
			lastalignement += simdt;
			//otherwise we are aligned, but still need to check every now and then if the alignement still holds
			if (lastalignement > 5.0 && !stopanimation)
			{
				lastalignement = 0;
				calculateTargetState();
				speed = abs(speed);
				secondaryspeed = abs(secondaryspeed);
				speed *= determineMovementDirection(primarytargetstate, state, data->components[0]->range >= 360);
				secondaryspeed *= determineMovementDirection(secondarytargetstate, secondarystate, data->components[1]->range >= 360);
			}
		}

		//update the state in orbiter
		if (enabled)
		{
			vessel->SetAnimation(orbiterid, state);
			vessel->SetAnimation(secondaryorbiterid, secondarystate);
		}
	}
	return statusevent;
}



bool Animation_Tracking::propagateState(double &_state, double _targetstate, double _speed, double simdt, bool canloop)
{
	if (_state != _targetstate)
	{
		double newstate = _state + (float)(simdt / data->duration) * _speed;
		//since we can't expect to hit the state precisely, we have to check whether we will pass it with this modification
		if (_state <= _targetstate && newstate >= _targetstate ||
			_state >= _targetstate && newstate <= _targetstate)
		{
			//we would overshoot the target state if we would apply the full modification
			_state = _targetstate;
		}
		else
		{
			_state = newstate;

			if (canloop)
			{
				//loop the animations around if they pass their boundaries
				if (_state < 0.0)
				{
					_state = 1.0 + _state;
					//unfortunately, this makes it possible for us to skip the target state if its close to zero,
					//so we need a saveguard against that.
					if (_targetstate > _state)
					{
						_state = _targetstate;
					}
				}
				else if (_state > 1.0)
				{
					_state = _state - 1.0;
					if (_targetstate < _state)
					{
						_state = _targetstate;
					}
				}
			}
			else
			{
				//stop at boundaries
				_state = min(_state, 1.0);
				_state = max(_state, 0.0);
			}
		}
	}
	
	return _state == _targetstate;
}




AnimationEvent_Base *Animation_Tracking::StartAnimation(StartAnimationEvent *startevent)
{
	//cast dynamically so we can throw an error instead of crashing. A wrong config file can lead to a wrong event being passed here.
	StartTrackingAnimationEvent *e = dynamic_cast<StartTrackingAnimationEvent*>(startevent);
	stopanimation = false;
	if (e == NULL)
	{
		Olog::error("tracking animation hooked to ordinary animation trigger!");
	}
	else if (speed == 0.0 || e->GetTrackingTarget() != target)		//ignore the event if we're already tracking the same target
	{
		speed = abs(e->GetSpeed());
		secondaryspeed = speed;
		target = e->GetTrackingTarget();
		//calculate the state the animation has to reach to point at the target
		calculateTargetState();
		//modify the speed to be positive or negative, depending on which direction the animation has to move in
		speed *= determineMovementDirection(primarytargetstate, state, data->components[0]->range >= 360);
		secondaryspeed *= determineMovementDirection(secondarytargetstate, secondarystate, data->components[1]->range >= 360);

		return new AnimationStartedEvent(data->id, speed);
	}
	return NULL;
}


void Animation_Tracking::ModifyAnimation(ModifyAnimationEvent *modifyevent)
{
	//check if the animation is even running
	if (speed == 0.0)
	{
		return;
	}

	//cast dynamically so we can throw an error instead of crashing. A wrong config file can lead to a wrong event being passed here.
	ModifyTrackingAnimationEvent *e = dynamic_cast<ModifyTrackingAnimationEvent*>(modifyevent);
	if (e == NULL)
	{
		Olog::error("tracking animation hooked to ordinary animation trigger!");
	}
	else
	{
		//if the new speed is 0.0, ignore it. only the target will be modified
		double newspeed = e->GetSpeed();
		if (newspeed != 0.0)
		{
			speed = abs(e->GetSpeed());
			secondaryspeed = speed;
		}

		OBJHANDLE newtarget = e->GetTrackingTarget();
		if (newtarget != NULL && newtarget != target)
		{
			target = newtarget;
			//calculate the state the animation has to reach to point at the target
			calculateTargetState();
			//modify the speed to be positive or negative, depending on which direction the animation has to move in
			speed = abs(speed) * determineMovementDirection(primarytargetstate, state, data->components[0]->range >= 360);
			secondaryspeed = abs(secondaryspeed) * determineMovementDirection(secondarytargetstate, secondarystate, data->components[1]->range >= 360);
		}
	}
}

void Animation_Tracking::initMovingAnimation()
{
	VECTOR3 secrotaxis = secondaryaxis;
	//calculate the facing of the secondary axis
	Rotations::RotateVector(secrotaxis, primaryaxis, state * 2 * PI);

	//transform the axis of the second rotation to reflect the first one
	normalise(secrotaxis);
	secondrotation->axis = secrotaxis;
	initmovinganim = true;
}

void Animation_Tracking::calculateTargetState()
{
	VECTOR3 targetdir, targetpos, mainrotaxis;
	
	if (stopanimation)
	{
		//if the animation needs to stop, it needs to go back to its original facing. 
		primarytargetstate = 0.0;
		secondarytargetstate = 0.0;
		secondrotation->axis = secondaryaxis;
	}
	else if (target != NULL)
	{
		//get the directional vector from vessel to target
		oapiGetGlobalPos(target, &targetpos);
		vessel->Global2Local(targetpos, targetdir);
		normalise(targetdir);

		//black magic trigonomancy by vchamp, with some changes by jedidia

		veccpy(mainrotaxis, primaryaxis);
		VECTOR3 facerot = crossp(mainrotaxis, secondaryaxis);

		VECTOR3 projectionplane = Rotations::GetProjection(targetdir, mainrotaxis);
		double mainrotangle = acos(dotp(facerot, projectionplane) /
			(length(facerot) * length(projectionplane)));

		if (dotp(mainrotaxis, crossp(facerot, projectionplane)) < 0)
		{
			mainrotangle = mainrotangle * -1;
		}

		//the angle the primary rotation axis has to rotate to face the target, in radians
		double requiredangle = mainrotangle < 0 ? 2 * PI + mainrotangle : mainrotangle;
		//translate the angle to the state of the animation, using the animation's range.
		//NOTE: If the range isn't 360 degrees, this might well result in a target state > 1
		//this is intended and serves to indicate that the targeted point lies outside
		//the animations capabilities.
		double rangeinradians = abs(data->components[0]->range * Calc::RAD_PER_DEGREE);
		//it is possible that the range of the animation was defined as a negative value
		//in that case, we need to calculate the required angle in the other direction
		if (data->components[0]->range < 0)
		{
			requiredangle = 2 * PI - requiredangle;
		}
		primarytargetstate = requiredangle / rangeinradians;


		VECTOR3 secrotaxis, facedir;
		//calculate the facing of the secondary axis
		veccpy(secrotaxis, secondaryaxis);
		Rotations::RotateVector(secrotaxis, mainrotaxis, primarytargetstate * 2 * PI);

		//tranform the axis of the second rotation to reflect the first one
		normalise(secrotaxis);
		secondrotation->axis = secrotaxis;

		veccpy(facedir, facing);
		Rotations::RotateVector(facedir, mainrotaxis, primarytargetstate * 2 * PI);

		projectionplane = Rotations::GetProjection(targetdir, secrotaxis);
		double secondaryrotangle = acos(dotp(facedir, projectionplane) /
			(length(facedir) * length(projectionplane)));

		if (dotp(secrotaxis, crossp(facedir, projectionplane)) < 0)
		{
			secondaryrotangle = secondaryrotangle * -1;
		}

		requiredangle = secondaryrotangle < 0 ? 2 * PI + secondaryrotangle : secondaryrotangle;
		rangeinradians = abs(data->components[1]->range * Calc::RAD_PER_DEGREE);
		//it is possible that the range of the animation was defined as a negative value
		//in that case, we need to calculate the required angle in the other direction
		if (data->components[1]->range < 0)
		{
			requiredangle = 2 * PI - requiredangle;
		}
		//translate the angle to a corresponding animation state. See notes above 
		//on the same operation for primary axis for details.
		secondarytargetstate = requiredangle / rangeinradians;

	}

}




int Animation_Tracking::determineMovementDirection(double targetstate, double currentstate, bool canloop)
{
	// find the closest way to the requested position. i.e. determine which way the rotation has to go around
	//calculate the difference between current state and target state if the animation does not cross its starting point
	double normaldistance = targetstate - currentstate;
	//calculate the difference between current state and target state if the animation passes 1 and starts again
	double loopdistance = targetstate - (currentstate + 1);
	//check which way is shorter
	double shortestdistance;
	if (canloop)
	{
		if (abs(normaldistance) < abs(loopdistance))
		{
			//not crossing the starting point is shorter
			shortestdistance = normaldistance;
		}
		else
		{
			//crossing the starting point will be shorter
			shortestdistance = loopdistance;
		}
	}
	else
	{
		//the animation can't loop around, so it can't take a shortcut by moving over its starting point
		shortestdistance = normaldistance;
	}

	//return what direction we actually have to move in
	if (shortestdistance < 0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

void Animation_Tracking::StopAnimation()
{
	stopanimation = true;
	calculateTargetState();
	speed = abs(speed) * determineMovementDirection(primarytargetstate, state, data->components[0]->range >= 360);
	secondaryspeed = abs(speed) * determineMovementDirection(secondarytargetstate, secondarystate, data->components[1]->range >= 360);
}


void Animation_Tracking::EnableAnimation()
{
	//make sure the animation actually exists on the vessel
	if (animationcomponents.size() > 0)
	{
		vessel->SetAnimation(secondaryorbiterid, secondarystate);
		vessel->SetAnimation(orbiterid, state);
		if (speed != 0.0)
		{
			calculateTargetState();
		}
		enabled = true;
	}
}

void Animation_Tracking::DisableAnimation()
{
	//make sure the animation actually exists on the vessel
	if (animationcomponents.size() > 0)
	{
		vessel->SetAnimation(orbiterid, 0.0);
		vessel->SetAnimation(secondaryorbiterid, 0.0);
		enabled = false;
	}
}



string Animation_Tracking::GetScenarioLine()
{
	//if the animation is at origin, we don't need to save it to the scenario
	if (state == 0.0 && secondarystate == 0 && speed == 0.0)
	{
		return "";
	}

	//otherwise we need the id plus the current state and speed
	stringstream ss;
	ss << data->id << " " << state << " " << speed << " " << secondarystate << " " << secondaryspeed;

	if (target != NULL && !stopanimation)
	{
		//if the animation is currently tracking a target, add the name of the target
		char targetname[512];
		oapiGetObjectName(target, targetname, 512);
		ss << " " << targetname;
	}
	return ss.str();
}


void Animation_Tracking::InitStateFromScenario(vector<string> &line)
{
	if (line.size() < 5 || line.size() > 6)
	{
		//invalid line. No need to terminate anything, the animation will just be reset to origin
		Olog::error("Invalid state for tracking animation in scenario!");
	}
	else
	{
		//set the animation state to what's saved in the scenario
		state = atof(line[1].data());
		speed = atof(line[2].data());
		secondarystate = atof(line[3].data());
		secondaryspeed = atof(line[4].data());

		if (line.size() == 6)
		{
			//the animation has been saved while tracking a target
			target = oapiGetObjectByName((char*)line[5].c_str());
			//if the target is invalid, note it in the log and crash!
			if (target == NULL)
			{
				Olog::error("Invalid target for tracking animation in scenario! crashing to desktop gracefully");
				//do not handle this exception!
				throw runtime_error("ERROR: Invalid target for tracking animation in scenario! crashing to desktop gracefully");
				
			}
		}
		else
		{
			//the animation has been saved while retracting
			stopanimation = true;
		}
	}
}
