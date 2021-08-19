#pragma once

/**
 * Notifies a tracking animation to start tracking, and which target
 *
 * Triggers AnimationStartedEvent or AnimationFailedEvent in response, and after that TrackingStatusAnimationEvent() for every frame the animation is running.
 * Indistinguishable from StartAnimationEvent if aimed at a non-tracking animation.
 * \see AnimationStartedEvent
 * \see AnimationFailedEvent
 * \see TrackingAnimationStatusEvent
 * \see EventHandler.h
 */
class StartTrackingAnimationEvent :
	public StartAnimationEvent
{
public:
	/**
	 * \param animationid Identifier of the tracking animation the event is aimed at.
	 * \param _speed The speed at which the animation should track
	 * \param trackingtarget Handle of the object to track
	 * \note The speed of a tracking animation does not determine its direction. The animation will choose the direction itself.
	 */
	StartTrackingAnimationEvent(string animationid, double _speed, OBJHANDLE trackingtarget) : StartAnimationEvent(animationid, _speed), target(trackingtarget){};
	~StartTrackingAnimationEvent(){};

	/**
	 * \return Handle to the object the animation should track
	 */
	OBJHANDLE GetTrackingTarget(){ return target; };

private:
	OBJHANDLE target;			//!< Stores the handle to the target the animation should be tracking
};
