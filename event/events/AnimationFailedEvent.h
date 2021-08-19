#pragma once

/**
 * \brief Notifies when an animation could not be started.
 *
 * Triggered once on start by IMS_Animation_Base
 * Usually triggered because of unfulfilled dependencies of an animation
 * If starting the animation succeeded, a AnimationStartedEvent will be triggered instead.
 * \see AnimationStartedEvent
 * \see EventHandler.h
 */
class AnimationFailedEvent :
	public AnimationEvent_Base
{
public:
	/**
	 * \param animationid The id of the animation that failed
	 * \param _speed The speed and direction the animation was trying to start in
	 */
	AnimationFailedEvent(string animationid, double _speed) : AnimationEvent_Base(EVENTTYPE::ANIMATIONFAILEDEVENT, animationid, 0), speed(_speed){};
	~AnimationFailedEvent(){};

	/**
	 * \return The speed and direction the failed animation tried to start with
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
