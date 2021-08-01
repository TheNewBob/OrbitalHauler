#pragma once
class EventHandler;

/**
 * \brief Abstract base class for events
 *
 * derive all events from this class.
 * event classes should have a unique identifier which you should add to the enum in events.h,
 * and carry all neccessary information for the particular event with them.
 * \see EventHandler.h
 */
class Event_Base
{
public:

	friend class EventGenerator;
	friend class EventBroker;

	/**
	 * \param _type The identifier for this kind of event
	 * \param _delay The event will fire this many frames after its creation
	 */
	Event_Base(EVENTTYPE _type, unsigned int _delay = 0);
	virtual ~Event_Base();

	/**
	 * \brief Returns the EventHandler at which this event originated.
	 */
	EventHandler *GetCaller() { return caller; };
	
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
	 * \brief Allows to compare an event directly to an EVENTTYPE
	 * \return True if the event type of this matches type
	 */
	virtual bool operator==(EVENTTYPE type);
	
	/**
	 * \return The event pipe this event originated in.
	 *
	 * This is the event pipe the event was generated in,
	 * and is intended to be used by EventHandlers in case they
	 * need to determine where an event came from and where it's going.
	 */
	EVENTPIPE GetEventPipe() { return pipe; };

protected:
	unsigned int delay;				//!< How many calls of ProcessEvents() on the event generator the event should be delayed
	EventHandler *caller;			//!< Stores the EventHandler at which the event originated
	EVENTTYPE eventtype;			//!< The type of this event
	EVENTPIPE pipe;					//!< The pipe the event travels in (unless it was relayed). This is set by the EventHandler itself
	
	/**
	 * \brief Used by event generators to determine whether the event should be sent or not.
	 *
	 * Decrements the delay by 1 when called
	 * \return True if delay is 0, false otherwise
	 */
	virtual bool sendMe();				
};

