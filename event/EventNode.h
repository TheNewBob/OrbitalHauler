#pragma once



class EventSink;

/**
 * \brief Abstract base class of a node in the event pipe tree from which EventGenerator and EventSink derive
 * \see EventHandler.h
 */
class EventNode
{

	friend class EventHandler;

public:
	EventNode();
	virtual ~EventNode() = 0;

protected:
	EventHandler *host;								//!< The EventHandler hosting this EventNode
	vector<EventSink*> children;					//!< The event nodes connecting directly to this EventNode

	/**
	 * \brief registers an event sink as a child of this node
	 * \param sink The EventSink intended to connect to this node
	 */
	void RegisterEventSink(EventSink *sink);

	/**
	 * \brief unregisters an event sink. 
	 * \param sink The EventSink you want to disconnect from this node
	 * \note Does NOT delete the child that is being unregistered!
	 * \return True if succesful, false if the passed node was not a child of this node
	 */
	bool UnregisterEventSink(EventSink *sink);

	/**
	 * \brief Sends an Event to all children.
	 * 
	 * Aborts propagation if its containing EventHandler or one of the children's EventHandler consumes the event.
	 * \param _event The event being propagated through the event pipe
	 * \note must be overloaded and called from inheriting classes
	 * \return True if the event was consumed, false otherwise
	 */
	virtual bool processEvent(Event_Base *_event);

};

