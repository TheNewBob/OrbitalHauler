#include "core/Common.h"
#include "OpForwardDeclare.h"
#include "AnimationData.h"
#include "event/Events.h"
#include "Animation_Base.h"
#include "util/Calc.h"


Animation_Base::Animation_Base(ANIMATIONDATA *_data)
	: data(_data), state(0.0), speed(0)
{
}


Animation_Base::~Animation_Base()
{
}


void Animation_Base::InitStateFromScenario(vector<string> &line)
{
	if (line.size() != 3)
	{
		//invalid line. No need to terminate anything, the animation will just be reset to origin
		Olog::warn("Invalid state for animation in scenario!");
	}
	else
	{
		//set the animation state to what's saved in the scenario
		state = atof(line[1].data());
		speed = atof(line[2].data());
	}
}


void Animation_Base::AddAnimationToVessel(VESSEL4 *_vessel, int _meshindex, MATRIX3 moduleorientation, VECTOR3 modulelocalpos)
{

	vessel = _vessel;
	meshindex = _meshindex;

	//create animation on vessel and remember the orbiter ID 
	orbiterid = createAnim(0.0);
	Olog::trace("created animation %s %i", data->id.data(), orbiterid);

	//create the animation on the vessel. We need to store the handles for parent-child relations
	//and for proper deletion later on.
	for (UINT i = 0; i < data->components.size(); ++i)
	{
		ANIMATIONCOMPONENT_HANDLE parent = NULL;
		int parentidx = data->components[i]->parent;

		//check if the component has a parent
		if (parentidx > -1)
		{
			//sanity check in case the writer of the config file screwed up and declared a wrong order
			if (parentidx >= (int)animationcomponents.size())
			{
				Olog::error("parent-child animations must be declared in order!");
				return;
			}
			parent = animationcomponents[parentidx];
		}

		if (data->components[i]->type == "rotate")
		{
			animationcomponents.push_back(vessel->AddAnimationComponent(orbiterid, data->components[i]->duration[0], data->components[i]->duration[1],
				createRotationComponent(data->components[i], modulelocalpos, moduleorientation),
				parent));
		}
		else if (data->components[i]->type == "translate")
		{
			animationcomponents.push_back(vessel->AddAnimationComponent(orbiterid, data->components[i]->duration[0], data->components[i]->duration[1],
				createTranslationComponent(data->components[i], modulelocalpos, moduleorientation),
				parent));
		}
		else if (data->components[i]->type == "scale")
		{
			animationcomponents.push_back(vessel->AddAnimationComponent(orbiterid, data->components[i]->duration[0], data->components[i]->duration[1],
				createScaleComponent(data->components[i], modulelocalpos, moduleorientation),
				parent));
		}
	}
	EnableAnimation();
}


void Animation_Base::RemoveAnimationFromVessel()
{
	Olog::debug("Deleting animation %s %i", data->id.data(), orbiterid);

	DisableAnimation();
	//delete the animationcomponents in reverse order.
	//technically DelAnimation should do that, but it 
	//seems it doesn't get the order right and crashes.
	for (UINT i = animationcomponents.size(); i > 0; --i)
	{
		bool result = vessel->DelAnimationComponent(orbiterid, animationcomponents[i - 1]);
		Olog::assertThat([result]() { return result; }, "Failed to remove animation from vessel!");
	}
	animationcomponents.clear();
	bool result = vessel->DelAnimation(orbiterid);
//	assert(result);

	//delete the meshgroup identifiers
	//for some reason, deleting them before the animation can cause a crash.
	for (UINT i = 0; i < animationgroups.size(); ++i)
	{
		delete[] animationgroups[i];
	}
	animationgroups.clear();
	orbiterid = -1;

}



MGROUP_ROTATE *Animation_Base::createRotationComponent(ANIMCOMPONENTDATA *comp, VECTOR3 modulelocalpos, MATRIX3 moduleorientation)
{
	//create an array for the groups and copy them over
	UINT *groups = new UINT[comp->groups.size()];
	for (UINT j = 0; j < comp->groups.size(); ++j)
	{
		groups[j] = comp->groups[j];
	}
	animationgroups.push_back(groups);

	//get the rotation reference and transform it to the modules alignement within the vessel
	VECTOR3 rotreference = comp->reference;
	rotreference = mul(moduleorientation, rotreference);
	Calc::RoundVector(rotreference, 1000);
//	rotreference += modulelocalpos;

	//doing the same with the rotation axis
	VECTOR3 rotaxis = comp->axis;
	rotaxis = mul(moduleorientation, rotaxis);
	normalise(rotaxis);


	//bunch it all together into an MGROUP_ROTATE object
	return new MGROUP_ROTATE(
		meshindex,
		groups, comp->groups.size(),
		rotreference,
		rotaxis,
		(float)Calc::RAD_PER_DEGREE * comp->range);
}



MGROUP_TRANSLATE *Animation_Base::createTranslationComponent(ANIMCOMPONENTDATA *comp, VECTOR3 modulelocalpos, MATRIX3 moduleorientation)
{
	//create an array for the groups and copy them over
	UINT *groups = new UINT[comp->groups.size()];
	for (UINT j = 0; j < comp->groups.size(); ++j)
	{
		groups[j] = comp->groups[j];
	}
	animationgroups.push_back(groups);

	//rotate the translation vector to the module's alignement
	VECTOR3 translation = comp->reference;
	translation = mul(moduleorientation, translation);
	Calc::RoundVector(translation, 1000);


	//bunch it all together into an MGROUP_ROTATE object
	return new MGROUP_TRANSLATE(
		meshindex,
		groups, comp->groups.size(),
		translation);
}



MGROUP_SCALE *Animation_Base::createScaleComponent(ANIMCOMPONENTDATA *comp, VECTOR3 modulelocalpos, MATRIX3 moduleorientation)
{
	//create an array for the groups and copy them over
	UINT *groups = new UINT[comp->groups.size()];
	for (UINT j = 0; j < comp->groups.size(); ++j)
	{
		groups[j] = comp->groups[j];
	}
	animationgroups.push_back(groups);

	//get the scaling origin and transform it to the modules alignement within the vessel
	VECTOR3 origin = comp->reference;
	origin = mul(moduleorientation, origin);
	Calc::RoundVector(origin, 1000);

	//doing the same with the actual scale factor (axis doubles for scale at this point in the ANIMATIONCOMPONENT struct)
	VECTOR3 scale = comp->axis;
	scale = mul(moduleorientation, scale);
	Calc::RoundVector(scale, 1000);


	//bunch it all together into an MGROUP_ROTATE object
	return new MGROUP_SCALE(
		meshindex,
		groups, comp->groups.size(),
		origin,
		scale);

}



AnimationEvent_Base *Animation_Base::StartAnimation(StartAnimationEvent *startevent)
{
	//check if the animation isn't running already
	if (speed == 0)
	{
		double _speed = startevent->GetSpeed();
		//check if the passed speed is within bounds
		if ((_speed < 0.0 || _speed > 0.0) &&
			abs(_speed) <= maxspeed)
		{
			//make the animation run at the specified speed in the specifeid direction
			speed = _speed;
		}
		else if (_speed == 0.0)
		{
			//start animation at default speed if it's not at the end. Otherwise, start it in reverse
			if (state < 1.0)
			{
				speed = 1.0;
			}
			else
			{
				speed = -1.0;
			}
		}
		return new AnimationStartedEvent(data->id, speed);
	}
	return NULL;
}


void Animation_Base::ModifyAnimation(ModifyAnimationEvent *modifyevent)
{
	double _speed = modifyevent->GetSpeed();
	//check if the passed speed is within bounds
	if ((_speed < 0.0 || _speed > 0.0) &&
		abs(_speed) <= maxspeed)
	{
		//make the animation run at the specified speed in the specifeid direction
		speed = _speed;
	}
}


void Animation_Base::StopAnimation()
{
	speed = 0;
}

string Animation_Base::GetScenarioLine()
{
	//if the animation is at origin, we don't need to save it to the scenario
	if (state == 0.0 && speed == 0.0)
	{
		return "";
	}

	//otherwise we need the id plus the current state and speed
	stringstream ss;
	ss << data->id << " " << state << " " << speed;
	return ss.str();
}


void Animation_Base::DisableAnimation()
{
	//make sure the animation actually exists on the vessel
	if (animationcomponents.size() > 0)
	{
		vessel->SetAnimation(orbiterid, 0.0);
		enabled = false;
	}
}

void Animation_Base::EnableAnimation()
{
	//make sure the animation actually exists on the vessel
	if (animationcomponents.size() > 0)
	{
		vessel->SetAnimation(orbiterid, state);
		enabled = true;
	}
}

UINT Animation_Base::createAnim(double initial_state) {
	//get the list of existing animations on the vessel
	ANIMATION* animlist;
	UINT nanim = vessel->GetAnimPtr(&animlist);

	//check the list for deleted entries (entries without a component).
	for (UINT i = 0; i < nanim; ++i)
	{
		if (animlist[i].ncomp == 0)
		{
			//an animation without components has been found.
			//return its id so it will be overwritten by the new animation's components
			animlist[i].defstate = initial_state;
			return i;
		}
	}

	//if there are no empty animations, create one
	UINT retvalue = vessel->CreateAnimation(initial_state);
	nanim = vessel->GetAnimPtr(&animlist);
	return retvalue;
}