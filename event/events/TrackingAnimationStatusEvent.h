#pragma once

/**
 * \file TrackingAnimationStatusEvent.h
 */

/**
 * \brief Describes the possible states a tracking animation can be in
 */
typedef enum
{
	ALIGNED,				//!< The tracking animation is aligned with its target
	ALIGNING,				//!< The tracking animation has a target and is aligning itself
	REVERTING,				//!< The tracking animation has been stoped and is reverting to its original position
	STOPPED,				//!< The tracking animation is not running
	UNABLE					//!< The tracking animation can not align itself with the chosen target
} TRACKINGANIMATIONSTATUS;

/**
 * \brief Event that carries the current status of a tracking animation.
 *
 * This event is triggered by a tracking animation for every frame the animation is running.
 * \see EventHandler.h
 * \todo add current pointing vector of the animation to the information carried by this event
 */
class TrackingAnimationStatusEvent :
	public AnimationEvent_Base
{
public:
	/** 
	 * \param animationid Identifier of the animation sending the event
   	 * \param trackingtarget The objhandle of the object to track
	 * \param _status The current status of the tracking animation
	*/
	TrackingAnimationStatusEvent(string animationid, OBJHANDLE trackingtarget, TRACKINGANIMATIONSTATUS _status) 
		: AnimationEvent_Base(EVENTTYPE::TRACKINGANIMATIONSTATUSEVENT, animationid, 1), target(trackingtarget), status(_status){};
	~TrackingAnimationStatusEvent(){};

	/**
	 * \return Handle to the target the animation is currently tracking
	 */
	OBJHANDLE GetTrackingTarget(){ return target; };
	
	/**
	 * \return The current status of the animation
	 */
	TRACKINGANIMATIONSTATUS GetStatus() { return status; };


private:
	OBJHANDLE target;								//!< Stores the target the animation is currently tracking
	TRACKINGANIMATIONSTATUS status;					//!< stores the current tracking status of the animation
};