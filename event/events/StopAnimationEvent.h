#pragma once


/**
 * Tells an animation to stop. Works for any animation type.
 *
 * Triggers AnimationFinishedEvent in response eventually, though when it will fire depends on the type of animation
 * \see AnimationFinishedEvent
 * \see EventHandler.h
 */
class StopAnimationEvent :
	public AnimationEvent_Base
{
public:
	/**
	 * \param animationid The identifier of the animation that should stop
	 */
	StopAnimationEvent(string animationid) : AnimationEvent_Base(EVENTTYPE::STOPANIMATIONEVENT, animationid, 0){};
	~StopAnimationEvent(){};

};

