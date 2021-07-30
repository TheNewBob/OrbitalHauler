#pragma once


class EventHandler;

/**
* receives and forwards events to its containing EventHandler and its connected EventSinks
*
* EventSinks receive an event, pass it to the containing EventHandler's ProcessEvent() function, and then forward it to any eventsinks connected to it.
*
* This class is pure private and cannot be instantiated or used directly. Use the interface provided by EventHandler instead.
* \see EventHandler.h
*/
class EventSink	: public EventNode
{
	friend class EventNode;
	friend class EventHandler;

private:
	/**
	 * \param _host The EventHandler containing this EventSink
	 */
	EventSink(EventHandler *_host);
	~EventSink();
	
	/**
	 * \brief Receives and propagates an event
	 *
	 * First sends the event to the ProcessEvent function of its host, then forwards it to all its children.
	 * Immediately breaks if the event was consumed down the line without further forwarding.
	 * \param e The event to be propagated
	 * \return True if the event was consumed, false otherwise
	 */
	bool processEvent(Event_Base *e);
};

