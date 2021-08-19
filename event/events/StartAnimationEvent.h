#pragma once

/**
 * \brief Notifies an animation to start
 * 
 * Triggers either AnimationStartedEvent or AnimationFailedEvent in response
 * \see AnimationStartedEvent
 * \see AnimationFailedEvent
 * \see StopAnimationEvent
 * \see EventHandler.h
 */
class StartAnimationEvent :
	public AnimationEvent_Base
{
public:
	/**
	 * \param animationid Identifier of the animation that should start
	 * \param _speed The speed/direction the animation should start with
	 */
	StartAnimationEvent(string animationid, double _speed) : AnimationEvent_Base(EVENTTYPE::STARTANIMATIONEVENT, animationid, 0), speed( _speed){};
	~StartAnimationEvent(){};

	/**
	 * \return The speed/direction the animation will start with
	 */
	double GetSpeed() { return speed; };

protected:
	/**
	* \brief An abstract property for how fast the animation should play, and in which direction.
	*
	* The value determines how fast in relation to the defined duration of the animation in the config file (1 == the exact duration)
	* The sign determines the direction (> 0: normal, < 0 : in reverse). For 0, see note
	* \note A speed of 0 does not mean stop! A speed of 0 means default speed (1) forward by default,
	*	except if the animation is already at the end, then it means in reverse. Basically, it means "start the animation in the most likely
	*	direction at tis default speed"
	*/
	double speed;
};

