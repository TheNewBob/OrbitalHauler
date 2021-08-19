#pragma once

/**
 * \brief Event that signals a tracking animation that the target has been modified or it should change its speed/direction
 *
 * Does not trigger a direct response.
 * Is indistinguishable from ModifyAnimationEvent when aimed at a non-tracking animation
 * \see TrackingAnimationStatusEvent
 * \see EventHandler.h
 */
class ModifyTrackingAnimationEvent :
	public ModifyAnimationEvent
{
public:
	/**
	 * \param animationid: Identifier of the tracking animation the event is aimed at
	 * \param _speed The speed the animation should move at
	 * \param trackingtarget The objhandle of the object to track
	 */
	ModifyTrackingAnimationEvent(string animationid, double _speed, OBJHANDLE trackingtarget) : ModifyAnimationEvent(animationid, _speed), 
		target(trackingtarget){};
	~ModifyTrackingAnimationEvent(){};

	/**
	 * \return A handle to the object the animation should be tracking
	 */
	OBJHANDLE GetTrackingTarget(){ return target; };

private:
	OBJHANDLE target;						//!< Stores the target the animation should start tracking
};
