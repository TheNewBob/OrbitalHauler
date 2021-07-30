#include "core/Common.h"
#include "Events.h"

UINT EventHandler::GetNewUID()
{
	static UINT uid = 0;
	uid++;
	return uid;
}

EventHandler::EventHandler()
{
	uid = GetNewUID();
}


EventHandler::~EventHandler()
{
	for (map<EVENTPIPE, EventSink*>::iterator i = eventsinks.begin(); i != eventsinks.end(); ++i)
	{
		delete i->second;
	}
	for (map<EVENTPIPE, EventGenerator*>::iterator i = eventgenerators.begin(); i != eventgenerators.end(); ++i)
	{
		delete i->second;
	}

	//if a waiting queue was used, destroy the generator and sink
	if (waiting_queue != NULL)
	{
		delete waiting_queue;
	}
	if (waiting_sink != NULL)
	{
		delete waiting_sink;
	}
}


/* connects an EventSink to the EventGenerator of this in the appropriate pipe
 * sink: the EventSink to be connected to an EventGenerator in this handler instance
 * pipe: The event pipe the sink should hook into
 */
void EventHandler::ConnectToMyEventGenerator(EventSink *sink, EVENTPIPE pipe)
{
	map<EVENTPIPE, EventGenerator*>::iterator i = eventgenerators.find(pipe);
	Olog::assertThat([i, this]() { return i != eventgenerators.end(); }, "The generator forming the start of the event pipe is not located in this instance!");

	i->second->RegisterEventSink(sink);
}

/* connects an EventSink on this to a passed event sink
 * sink: the EventSink to be connected to a sink in this handler instance
 * pipe: The event pipe the sink should hook into
 */
void EventHandler::ConnectToMyEventSink(EventSink *sink, EVENTPIPE pipe)
{
	map<EVENTPIPE, EventSink*>::iterator i = eventsinks.find(pipe);
	Olog::assertThat([i, this]() { return i != eventsinks.end(); }, "A sink being a part of this event pipe must be located in this instance!");

	i->second->RegisterEventSink(sink);
}


/* Connects an EventSink on this to the passed EventNode, no matter whether generator or another sink.
 * In short, this forms a connection in opposite direction from ConnectToMyEventGenerator and ConnectToMyEventSink
 * generator: the EventGenerator the Sink on this should connect to
 * pipe: The Eventpipe the Sink should be a part of. A sink of this pipe must be present on this!
 */
void EventHandler::ConnectMySinkToYourNode(EventNode *node, EVENTPIPE pipe)
{
	map<EVENTPIPE, EventSink*>::iterator i = eventsinks.find(pipe);
	Olog::assertThat([i, this]() { return i != eventsinks.end(); }, "A sink that is part of the event pipe must be located in this instance!");

	node->RegisterEventSink(i->second);
}





/* creates a new EventGenerator in this handler instance
 * pipe: The event pipe this generator should be at the beginning of
 */
EventGenerator *EventHandler::createEventGenerator(EVENTPIPE pipe)
{
	Olog::assertThat([this, pipe]() { return eventgenerators.find(pipe) == eventgenerators.end(); }, "You're trying to create two generators belonging to the same pipe!");
	Olog::assertThat([pipe]() { return pipe != WAITING_PIPE; }, "WAITING_PIPE is a reserved pipe and has its own generator!");
	EventGenerator *newgenerator = new EventGenerator(this, pipe);
	eventgenerators.insert(make_pair(pipe, newgenerator));
	return newgenerator;
}

/* creates a new EventSink in this handler instance
 * pipe: The event pipe this sink should hook into
 */
EventSink *EventHandler::createEventSink(EVENTPIPE pipe)
{
	Olog::assertThat([this, pipe]() { return eventsinks.find(pipe) == eventsinks.end(); }, "You're trying to create two sinks belonging to the same pipe!");
	EventSink *neweventsink = new EventSink(this);
	eventsinks.insert(make_pair(pipe, neweventsink));
	return neweventsink;
}


/* returns the event sink that belongs to the passed event pipe
 * returns NULL if no sink in this pipe exists in the EventHandler
 */
EventSink *EventHandler::getEventSink(EVENTPIPE pipe)
{
	map<EVENTPIPE, EventSink*>::iterator i = eventsinks.find(pipe);
	if (i != eventsinks.end())
	{
		return i->second;
	}
	return NULL;
}


/* returns the event generator that belongs to the passed event pipe
 * returns NULL if no sink in this pipe exists in the EventHandler
 */
EventGenerator *EventHandler::getEventGenerator(EVENTPIPE pipe)
{
	map<EVENTPIPE, EventGenerator*>::iterator i = eventgenerators.find(pipe);
	if (i != eventgenerators.end())
	{
		return i->second;
	}
	return NULL;
}


/* adds an event to the eventcue of an eventpipe
 * _event: The eventtype of the event to be generated
 * _pipe: The event pipe the event should be added to. if _pipe == DEFAULT_PIPE, the event will be sent along this handlers default pipe (i.e. the first generator in the list)
 */
void EventHandler::addEvent(Event_Base *_event, EVENTPIPE _pipe)
{
	Olog::assertThat([this]() { return eventgenerators.size() > 0; }, "Adding event to cue with no generators!");
	//if the default pipe is not configured, the default pipe defaults to the first pipe in the map (that sentence has way too many defaults in it!!)
	if (_pipe == DEFAULT_PIPE && defaultpipe == DEFAULT_PIPE)
	{
		if (_pipe == DEFAULT_PIPE)
		{
			eventgenerators.begin()->second->GenerateEvent(_event);
		}
	}
	else if (_pipe == DEFAULT_PIPE)
	{
		//if the default pipe is configured, reroute an event sent to the default pipe to the pipe the default pipe is mapped to.
		_pipe = defaultpipe;
	}
	else if (_pipe == WAITING_PIPE)
	{
		//if the event is sent to the waiting queue, it is sent to a special eventgenerator
		waiting_queue->GenerateEvent(_event);
	}
	
	if (_pipe != DEFAULT_PIPE)
	{
		map<EVENTPIPE, EventGenerator*>::iterator i = eventgenerators.find(_pipe);
		Olog::assertThat([i, this]() { return i != eventgenerators.end(); }, "No event generator defined for pipe!");
		i->second->GenerateEvent(_event);
	}
}


void EventHandler::relayEvent(Event_Base *_event, EVENTPIPE _pipe)
{
	Olog::assertThat([this, _pipe]() { return eventgenerators.find(_pipe) != eventgenerators.end(); }, "you're trying to relay to an event pipe that has no generator in this eventhandler!");

	eventgenerators.find(_pipe)->second->RelayEvent(_event);
}

/* tells all event generators in this event handler to advance their event cue */
void EventHandler::sendEvents()
{
	for (map<EVENTPIPE, EventGenerator*>::iterator i = eventgenerators.begin(); i != eventgenerators.end(); ++i)
	{
		(*i).second->ProcessEvents();
	}
}

/* completely severs connections to and from the passed EventHandler. All sinks, all generators.
 * handler: the EventHandler instance to disconnect
 * bidirectional: always pass true (default)! this is only used to call the function on the opposing handler
 */
void EventHandler::disconnect(EventHandler *handler, bool bidirectional)
{
	//throw all our event nodes into one collection
	vector<EventNode*> nodes;
	MapToVector(eventsinks, nodes);
	MapToVector(eventgenerators, nodes);

	//walk through all eventnodes hosted by this handler
	for (UINT i = 0; i < nodes.size(); ++i)
	{
		EventNode *node = nodes[i];
		//walk through all children registered at the node
		for (UINT j = 0; j < node->children.size(); ++j)
		{
			//if one of the children is hosted by the passed EventHandler, unregister it
			if (node->children[j]->host == handler)
			{
				node->UnregisterEventSink(node->children[j]);
			}
		}
	}

	//Now do the same from the other side
	if (bidirectional)
	{
		handler->disconnect(this, false);
	}
}


void EventHandler::addEventToWaitingQueue(Event_Base *_event)
{
	if (waiting_queue == NULL || waiting_sink == NULL)
	{
		createWaitingQueue();
	}
	waiting_queue->GenerateEvent(_event);
}


void EventHandler::processWaitingQueue()
{
	//check if this handler actually ever had any events added to its waiting queue
	if (waiting_queue == NULL || waiting_sink == NULL) return;
	waiting_queue->ProcessEvents();
}

void EventHandler::createWaitingQueue()
{
	if (waiting_queue == NULL)
	{
		waiting_queue = new EventGenerator(this, WAITING_PIPE);
		waiting_queue->ignoredelay = true;
	}

	if (waiting_sink == NULL)
	{
		waiting_sink = new EventSink(this);
	}

	waiting_queue->RegisterEventSink(waiting_sink);
}