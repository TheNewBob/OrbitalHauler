#pragma once


/**
 * \brief This is a base class for events that should fire after a set time interval in milliseconds instead of a framecount.
 *
 * This still assumes that an event generator will check the event every frame. The event won't send itself just because it exists!
 * \note: Though time can be defined in miliseconds, the interval will not be sent at milisecond precision. It is not asynchronous and
 * the precision of the timer very much depends on the framerate, so don't use this to control precision-critical operations!
 * \note The event counts system time, not sim time, and will therefore be unaffected by time acceleration.
 * \see EventHandler.h
 */
class Event_Timed :
	public Event_Base
{
public:
	/**
	 * \param _type The event type of this event
	 * \param miliseconds In how many miliseconds the event should fire
	 */
	Event_Timed(EVENTTYPE _type, unsigned int miliseconds);
	~Event_Timed();

protected:
	/**
	 * \brief Checks how much time has elapsed since the creation of the event
	 * \return True if the set interval has passed, false otherwise
	*/
	virtual bool sendMe();

private:
	unsigned long int creationtime;						//!< stores the system time at which the event was created.
};

