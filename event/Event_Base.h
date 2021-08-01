#pragma once
class EventHandler;

/**
 * \brief Abstract base class for events
 *
 * derive all events from this class.
 * event classes should have a unique identifier which you should add to the enum in events.h.
 * If you don't need any data attached to your event, use SimpleEvent. For more complex events that transfer data,
 * inherit this class and attach your data as necessary.
 */
class Event_Base
{
public:

	friend class EventBroker;

	/**
	 * \param _type The identifier for this kind of event
	 * \param _delay The event will fire this many frames after its creation
	 */
	Event_Base(EVENTTYPE type, unsigned int delay = 0);
	virtual ~Event_Base();

	/**
	 * \brief Compare whether two events are the same.
	 *
	 * this comparison usually depends on event type, but may have to consider
	 * other properties depending on the kind of event.
	 * Overload to inheriting classes if necessary.
	 * Most common events are the same event if they are of the same type. 
	 * Events that carry additional information might need more distinction!
	 * \return True if the event types of both events match, false otherwise
	 */
	virtual bool operator==(Event_Base *e);

	/**
	* \return !operator==(Event_Base *e)
	*/
	virtual bool operator!=(Event_Base *e);

	/**
	 * \brief Allows to compare an event directly to an EVENTTYPE with the syntax *event == type.
	 * \return True if the event type of this matches type.
	 */
	virtual bool operator==(EVENTTYPE type);
	

protected:
	unsigned int delay;				//!< How many calls of ProcessEvents() on the event generator the event should be delayed
	EVENTTYPE eventtype;			//!< The type of this event
	
	/**
	 * \brief Used by event generators to determine whether the event should be sent or not.
	 *
	 * Decrements the delay by 1 when called
	 * \return True if delay is 0, false otherwise
	 */
	virtual bool sendMe();				
};

