#pragma once

/**
 * \brief Notifies that an animation has finished
 *
 * Triggered by the animation when it stops.
 * The exact point of the animation stop depends on the type of animation.
 * While Sequential animations are able to stop at any moment, Both Continuous
 * and tracking animations will first return to their original state upon 
 * receiving a StopAnimationEvent, and will only fire this event once they are
 * savely back where they started.
 * \see StopAnimationEvent
 * \see EventHandler.h
 */
class AnimationFinishedEvent :
	public AnimationEvent_Base
{
public:
	/**
	 * \param animationid Identifier of the animation that finished
	 */
	AnimationFinishedEvent(string animationid) : AnimationEvent_Base(EVENTTYPE::ANIMATIONFINISHEDEVENT, animationid, 0){};
	~AnimationFinishedEvent(){};

};

