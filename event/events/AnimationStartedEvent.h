#pragma once

/**
 * \brief Notifies that an animation has been started. 
 *
 * Not to be confused with StartAnimationEvent, which notifies that an animation
 * SHOULD be started. In fact, this event is the nominal response to StartAnimationEvent
 * Triggered Once on start by IMS_Animation_Base
 * If the animation is for some reason unable to start, it will trigger an AnimationFailedEvent instead
 * \see AnimationFailedEvent
 * \see EventHandler.h
 */
class AnimationStartedEvent :
	public AnimationEvent_Base
{
public:
	/**
	 * \param animationid Identifier of the animation triggering the event
	 * \param _speed starting speed of the animation triggering the event
	 */
	AnimationStartedEvent(string animationid, double _speed) : AnimationEvent_Base(EVENTTYPE::ANIMATIONSTARTEDEVENT, animationid, 0), speed(_speed){};
	~AnimationStartedEvent(){};

	/**
	 * \return The speed and direction the animation started with
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
