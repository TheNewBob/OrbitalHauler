#pragma once

/**
* \brief EventGenerators take on events, equip them with additional information about the sender, and process them according to their urgency.
*
* Events that have no delay will be fed into the eventpipe immediately at the moment of their creation. Otherwise an event will be added
* to the generators event queue, which is advanced every time the ProcessEvents() method on the generator is called, which is called 
* from EventHandler::sendEvents()).
*
* This class is pure private. It cannot be instantiated or used directly. Instead, use the interface provided by EventHandler.
* \see EventHandler.h
*/
class EventGenerator : public EventNode
{
	friend class EventHandler;
private:
	/**
	 * \param _host The EventHandler instance that is hosting this EventGenerator
	 * \param _pipe The event pipe this generator should generate events for
	 * \param ignoredelay A generator with this set to true will completely ignore event delays,
	 *	always processing the whole queue at the next call to ProcessEvents().
	 */
	EventGenerator(EventHandler *_host, EVENTPIPE _pipe);
	~EventGenerator();
	
	/**
	* \brief Pass an event to this function to send it down the eventpipe. 
	*
	* Depending whether the event is instantaneous or delayed, it gets processed immediately
	* or added to the event queue, which will be emptied the next time ProcessEvents is called on this EventGenerator
	* \param _event The event to be sent through the pipe
	* \param ignoredelay If true, an event with delay 0 is not generated immediately and instead added to the event queue
	* \note The _event pointer will be deallocated by the generator when it has finished its run through the pipe.
	*/
	void GenerateEvent(Event_Base* _event);

	/**
	 * \brief Relays an event to this event pipe
	 *
	 * The event pipes are the usual lines of communication.
 	 * However, every now and then it may be necessary for an event
	 * to jump pipes. The necessity for this has to be determined
	 * by the ProcessEvent implementation of the respective EventHandler.
	 * Then the event can be relayed to another eventgenerator using this method,
	 * which will propagate the event instantly through its pipe. This means
	 * that the event will propagate through this entire pipe first, before
	 * propagation through the original pipe is resumed.
	 * RelayEvent will NOT delete the event even if it was consumed,
	 * but it will return whether the event was consumed. If it was,
	 * the relaying ProcessEvent implementation has to decide whether to
	 * consume the event itself or continue propagation through its own pipe.
	 * \param _event The event to be relayed.
	 * \note If you pass an event to this function that did not come from another eventpipe,
	 *	you create a memory leak!
	 */
	bool RelayEvent(Event_Base *_event);
	
	/**
	 * \brief Goes through the event queue and sends out the events that are due.
	 *
	 * if an event is not due, it will stay in the queue until it is.
	 */
	void ProcessEvents();

	/**
	 * \brief sends an event down the pipe of this generator.
	 * 
	 * Also receives the response when propagation has finished and deallocates the event..
	 * \param e The event to be processed.
	 * \note All pointers to the event will be invalid after this call
	 */
	bool processEvent(Event_Base *e);

	deque<Event_Base*> eventqueue;					//!< The eventqueue, containing all events that are waiting to be sent out.
	EVENTPIPE pipe;									//!< Identifier of the event pipe this generator is part of
	bool ignoredelay = false;
};

