#pragma once

/**
 * \brief Base event for all animation related events. No intended to be used directly
 *
 * Animation events are special in that they carry the id of the animation they are aimed at. Animations will not listen to events
 * that are not specifically addressed at them.
 */
class AnimationEvent_Base :
	public Event_Base
{
public:
	/**
	 * \param animationid The string identifier of the animation the event is aimed at
	 * \param type The event type fo the event
	 * \param _delay How many frames the event should be delayed
	*/
	AnimationEvent_Base(EVENTTYPE type, string animationid, int _delay) : Event_Base(type, _delay), animid(animationid){};
	virtual ~AnimationEvent_Base(){};

	/**
	 * \return The string identifer of the animation this event is aimed at
	 */
	string GetAnimationId() { return animid; };

	/** 
	 * \param e The event to compare this event to
	 * \return True if the if this and e have the same event type and are aimed at the same animation, false otherwise
	 */
	virtual bool operator==(Event_Base *e)
	{
		AnimationEvent_Base *otherevent = dynamic_cast<AnimationEvent_Base*>(e);
		if (otherevent != NULL && otherevent->eventtype == eventtype)
		{
			return animid == otherevent->animid;
		}
		return false;
	}

protected:
	string animid;					//!< Stores the identifier of the animation this event is aimed at
};