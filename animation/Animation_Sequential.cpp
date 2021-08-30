#include "core/Common.h"
#include "OpForwardDeclare.h"
#include "AnimationData.h"
#include "event/Events.h"
#include "Animation_Base.h"
#include "Animation_Sequential.h"


Animation_Sequential::Animation_Sequential(ANIMATIONDATA *_data) : Animation_Base(_data)
{
}


Animation_Sequential::~Animation_Sequential()
{
}


AnimationEvent_Base *Animation_Sequential::PropagateAnimation(double simdt)
{


	//check if the animation is running at all
	if (speed != 0.0)
	{
		//calculate how much the state propagates in the passed time and update it
		//this is where the abstract speed value becomes an actual speed in combination with the animation's default duration
		//if speed is 1 or -1, the animation will need duration seconds to complete. 
		state += (float)(simdt / data->duration) * speed;

		//check if the animation has finished or not, in both directions
		if (state <= 0 || state >= 1)
		{
			//set the state to its boundaries and stop the animation
			state = max(state, 0);
			state = min(state, 1);
			speed = 0.0;
		}

	
		//set the state of the animation in orbiter if the visual exists
		if (enabled)
		{
			vessel->SetAnimation(orbiterid, state);
		}
		
		//if the animation has finished, return the event.
		if (speed == 0.0)
		{
			return new AnimationFinishedEvent(data->id, state);
		}
	}

	return NULL;
}


