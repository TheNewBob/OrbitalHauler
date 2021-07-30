#pragma once

/**
 * \file EventHandler.h
 * General description of the Event engine
 * 
 * This event engine is built around two core principles: Provide clear and seperable lines of communication through a program,
 * while being easy and effortless to work with once those lines are laied out. Besides that it also had to be flexible enough to easily
 * sever and reestablish its lines of communication without complication. This goal is achieved by a three component architecture:
 *
 * EventHandlers are the base class from which classes that need to send or receive events have to derive. This base class provides an interface
 * to create EventGenerators and Sinks and connect as well as disconnect them, and to generate events for the appropriate event pipes. The EventHandler
 * class also manages the different event pipes running through it, although this doesn't need further input after laying out the initial lines
 * of communication. The most visible function of the EventHandler interface is to provide an overloadable ProcessEvent() function, which provides a central 
 * hub for a derived class to handle its events. All events passing through the EventHandler, no matter what pipe they belong to, will pass through this method.
 *
 * EventGenerators take on events and process them according to their properties. Depending on an events delay, the generator will either send an event
 * down its pipe immediately or add it to an eventqueue that is worked off at every call to sendEvents() of its containing EventHandler instance.
 * EventGenerators should always be created, connected and used through the interface of the EventHandler class, not directly.
 *
 * EventSinks are connected to event generators or other event sinks with the sole purpose of passing events down an event pipe and passing them to the ProcessEvent() 
 * function of their hosting EventHandler instance. They are themselves unable to generate events. EventSinks should always be created, connected and used through
 * the interface provided by the EventHandler class, not directly.
 *
 * The Event Pipe is the concept connecting these components together. An event pipe is not a class and doesn't exist as a formulated structure in the code.
 * It is an identifer that tells event generators and event sinks which line of communication they belong to. As a concept, an event pipe is a tree with one or several 
 * EventGenerators at its root (see below), branching out into an arbitrary number of event sinks all belonging to the same event pipe. When an EventGenerator is told to generate an event,
 * it will send that event to all event sinks that are connected to it. These sinks in turn will first send the event to the ProcessEvent() function of their EventHandler, and
 * then onwards to their own child sinks. EventHandlers can choose to consume an event, in which case the propagation is stopped.
 * Event Pipes are uni-directional: The event travels upwards from the generator, triggering all EventHandlers on its path, and returning true or false to signify consumption.
 * Bi-directional communication therefore requires two event pipes.
 *
 * It is possible, and often necessary, for an event pipe to have several eventgenerators. I.E. the tree has several roots. For example, if a class "Container" has a collection of
 * class instances of class "child", and the children need to send events to the Container, it makes sense for them to all use the same event pipe. Except of course they are all using
 * their own event generator. In these situations the important thing is that that all these generators connect to the same sink, So the path of the event is the same no matter where
 * the event comes from. Imagine a tree that has one stem (the first eventsink in the pipe), and branches out towards the top (further event sinks up the tree), but also branches
 * at the root (multiple event generators connecting to the "stem"). It's not strictly a tree in terms of data structures, but it's pretty close to a real tree, so it shouldn't be
 * difficult to imagine. Just keep in mind that the communication is still uni-directional, i.e. the event ALWAYS travels from root to top.
 */ 


class Event_Base;
class EventNode;
class EventSink;
class EventGenerator;


/**
 * \brief EventHandler base class. All classes that implement eventhandling must inherit from this class.
 *
 * For a more detailed description of the event engine as a whole, see the file documentation for EventHandler.h
 */
class EventHandler
{
	friend class EventSink;

public:
	EventHandler();
	virtual ~EventHandler();

	/**
	 * \brief connects an EventSink to the EventGenerator of this in the appropriate pipe.
	 * \param sink The EventSink to be connected to an EventGenerator in this handler instance
	 * \param pipe The event pipe the sink should hook into
	 */
	void ConnectToMyEventGenerator(EventSink *sink, EVENTPIPE pipe);
	
	/**
	 * \brief Connects an EventSink on this to the passed event sink
	 * \param sink The EventSink to be connected to a sink in this handler instance
	 * \param pipe The event pipe the sink should hook into
	 */
	void ConnectToMyEventSink(EventSink *sink, EVENTPIPE pipe);
	
	/**
	 * \brief Connects an EventSink on this to the passed EventNode, no matter whether generator or another sink.
   	 * 
	 * In short, this forms a connection in opposite direction from ConnectToMyEventGenerator and ConnectToMyEventSink
	 * \param node The EventGenerator the Sink on this should connect to
	 * \param pipe The Eventpipe the Sink should be a part of. A sink of this pipe must be present on this!
	 */
	void ConnectMySinkToYourNode(EventNode *node, EVENTPIPE pipe);

	/**
	 * \brief Returns the EVENTHANDLERTYPE of this event handler.
	 */
	EVENTHANDLERTYPE GetEventHandlerType() { return eventhandlertype; };
	
	/**
	 * \brief Returns the  globaly unique identifier of this event handler.
	 */
	UINT GetUID() { return uid; };

protected:
	EVENTPIPE defaultpipe = DEFAULT_PIPE;							//!< Use this to set the defualt event pipe of this event handler
	EVENTHANDLERTYPE eventhandlertype;								//!< An identifier that offers a group identification

	/**
	 * \brief Creates a new EventGenerator for this handler instance.
	 * \param pipe The event pipe this generator services
	 */
	EventGenerator *createEventGenerator(EVENTPIPE pipe);

	/**
	 * \brief creates a new EventSink in this handler instance
	 * \param pipe The event pipe this sink should hook into
	 */
	EventSink *createEventSink(EVENTPIPE pipe);

	/**
	 * \brief Completely severs connections to and from the passed EventHandler, all sinks, all generators.
	 * \param handler The EventHandler instance to disconnect
	 * \param bidirectional Always pass true (default)! This argument is only used to call the function on the opposing handler.
	 */
	void disconnect(EventHandler *handler, bool bidirectional = true);

	/**
	 * \brief Tells all event generators in this event handler to advance their event cue.
	 * \note Make sure this is called every frame for every EventHandler.
	 * \see sendImmediateEvents()
	 */
	void sendEvents();

	/**
	 * \brief processes all events in the waiting queue.
	 *
	 * The waiting queue stands outside the normal event processing and will only be handled when this method is called.
	 * All events in the waiting queue will be processed, regardless of delay, and will be destroyed once finished.
	 * The waiting queue will only send events to this eventhandlers processEvent() method.
	 */
	void processWaitingQueue();

	/**
	 * \brief Adds an event to the eventcue of an eventpipe
	 * \param _event A pointer to an event. The memory will be freed by the event handler once the event has been processed
	 * \param _pipe The event pipe the event should be added to. if _pipe == DEFAULT_PIPE, the event will be sent along this handlers default pipe (i.e. the first generator in the list).
	 * \note Do NOT add pointers to events received in processEvents(). Those pointers will be deallocated once the event
	 *	generator in question has finished its business! use RelayEvent() instead.
	 * \see addEventToWaitingQueue()
	 * \see relayEvent()
	 */
	void addEvent(Event_Base *_event, EVENTPIPE _pipe = DEFAULT_PIPE);

	/**
	* \brief Adds an event to this handlers waiting queue
	*
	* The waiting queue is a dedicated event generator that will not generate its events when sendEvents() is called,
	* but instead when processWaitingQueue is called.
	* \param _event A pointer to an event. The memory will be freed by the event handler once the event has been processed.
	* \note Do NOT add pointers to events received in processEvents(). Those pointers will be deallocated once the event
	*	generator in question has finished its business!
	*/
	void addEventToWaitingQueue(Event_Base *_event);

	/**
	 * \brief Relays an event to another event pipe.
	 * 
	 * Relaying means that the event will be propagated through 
	 * the designated pipe IMMEDIATELY, but it will not be consumed
	 * by it. The event will still exist after its detour, which would
	 * not be the case if it was simply added to another event pipe with no delay
	 * \param _event The event to relay. 
	 * \param _pipe The event pipe the event should be relayed to. 
	 * \note _event: Do not pass a a newly created event to this function!
	 *		use addEvent to propagate new events. If you relay a new event, you're creating
	 *		a memory leak! only relay events you get from ProcessEvent()!
	 * \note _pipe: An EventGenerator belonging to this pipe must exist in the EventHandler!
	 * \see AddEvent();
	 */
	void relayEvent(Event_Base *_event, EVENTPIPE _pipe);

	/**
	 * \brief returns the EventSink on this EventHandler that belongs to the passed event pipe
	 * \param pipe The event pipe of which to retrieve the sink
	 * \return NULL if no sink in this pipe exists in the EventHandler
	 */
	EventSink *getEventSink(EVENTPIPE pipe);
	
	/**
	 * \brief Returns the EventGenerator on this EventHandler that belongs to the passed event pipe
	 * \param pipe The event pipe of which to retrieve the sink
	 * \return NULL if no generator in this pipe exists in the EventHandler
	 */
	EventGenerator *getEventGenerator(EVENTPIPE pipe);

	/**
	 * \brief Receives all events coming through this EventHandler.
	 * 
	 * Overload to implement event handling in your inheriting class.
	 * The function will receive Events one by one as they are coming through
	 * your event sinks, regardless of which pipe they belong to.
	 * Return true to consume an event, but be sure it was only meant for you
	 * when you do that.
	 * \param e The event coming through the event pipe
	 * \note Event generators will NOT send events to this function, only sinks.
	 */
	virtual bool ProcessEvent(Event_Base *e) = 0;

private:
	UINT uid;														//!< Globally unique identier, handy mostly for debugging.
	map<EVENTPIPE, EventGenerator*> eventgenerators;				//!< Stores the event generators of this EventHandler
	map<EVENTPIPE, EventSink*> eventsinks;							//!< Stores the EventSinks of this EventHandler
	EventGenerator *waiting_queue = NULL;							//!< The event generator dedicated to processing the waiting queue.
	EventSink *waiting_sink = NULL;									//!< The event sink for the waiting queue

	/**
	 * \brief creates the generator and sink for the waiting queue
	 * \note The waiting queue is created on demand, since only few EventHandlers will need one.
	 *	Once created it will last for the remaining lifetime of the handler.
	 */
	void createWaitingQueue();

	// Generates a new unique identifier for an event handler (It's really just an incrementing UINT).
	static UINT GetNewUID();
	
	/**
	 * \brief Appends all values in a map to a vector, ignoring keys
	 * \param _map The map from which to copy the values
	 * \param _vector The vector to which the values should be appended
	 */
	template <typename M, typename V>
	void MapToVector(const  M& _map, V& _vector)
	{
		//resize vector first, it's faster this way
		UINT i = _vector.size();
		_vector.resize(i + _map.size());
		for (typename M::const_iterator it = _map.begin(); it != _map.end(); ++it, ++i)
		{
			_vector[i] = it->second;
		}
	}
};

