#include "core/Common.h"
#include "OpForwardDeclare.h"
#include "AnimationData.h"
#include "event/Events.h"
#include "Animation_Base.h"
#include "Animation_Continuous.h"


Animation_Continuous::Animation_Continuous(ANIMATIONDATA *_data) : Animation_Base(_data)
{
}


Animation_Continuous::~Animation_Continuous()
{
}


AnimationEvent_Base *Animation_Continuous::PropagateAnimation(double simdt)
{

	//check if the animation is running at all
	if (speed != 0.0)
	{
		//calculate how much the state propagates in the passed time and update it
		//this is where the abstract speed value becomes an actual speed in combination with the animation's default duration
		//if speed is 1 or -1, the animation will need duration seconds to complete. 
		state += (float)(simdt / data->duration) * speed;

		//check if the animation went over the boundary between 0 and 1, and loop it back on itself
		if (state <= 0)
		{
			if (stopmenexttime)
			{
				//the animation has been told to stop
				state = 0.0;
				speed = 0.0;
				stopmenexttime = false;
				return new AnimationFinishedEvent(data->id);
			}
			else
			{
				//if the state gets below zero, this also means that the animation is running in reverse,
				//so start at the "end" again.
				state = state + 1;
			}
		}
		else if (state >= 1)
		{
			if (stopmenexttime)
			{
				//the animation has been told to stop
				state = 0.0;
				speed = 0.0;
				stopmenexttime = false;
				return new AnimationFinishedEvent(data->id);
			}
			else
			{
				//reset the animation to the start, plus the minor offset of this state propagation
				state = state - 1;
			}
		}

		//set the state of the animation in orbiter if the visual exists
		if (enabled)
		{
			vessel->SetAnimation(orbiterid, state);
		}

	}

	//this animation only stops on outside input
	return NULL;
}



void Animation_Continuous::StopAnimation()
{
	//instead of immediately stopping the animation, it will proceed until its original position
	stopmenexttime = true;
}

void Animation_Continuous::InitStateFromScenario(vector<string> &line)
{
	if (line.size() >= 3  && line.size() < 5)
	{
		//set the animation state to what's saved in the scenario
		state = atof(line[1].data());
		speed = atof(line[2].data());
		if (line.size() == 4)
		{
			//the animation is in the process of stopping. We don't even have to evaluate the sign.
			stopmenexttime = true;
		}
	}
	else
	{
		//invalid line. No need to terminate anything, the animation will just be reset to origin
		Olog::warn("Invalid state for animation in scenario!");
	}
}


string Animation_Continuous::GetScenarioLine()
{
	string scenarioline = Animation_Base::GetScenarioLine();

	if (stopmenexttime)
	{
		//if the animation is in the process of stopping, note that in the scenario
		scenarioline += " !";
	}
	return scenarioline;
}