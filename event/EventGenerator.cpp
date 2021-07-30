#include "core/Common.h"
#include <deque>
#include "Events.h"


EventGenerator::EventGenerator(EventHandler *_host, EVENTPIPE _pipe)
{
	host = _host;
	pipe = _pipe;
	this->ignoredelay = ignoredelay;
}


EventGenerator::~EventGenerator()
{
}


/* this function accepts a new event. Depending whether it is instantaneous or delayed, it gets processed immediately
 * or added to the event queue, which will be emptied the next time ProcessEvents is called on this EventGenerator
 */
void EventGenerator::GenerateEvent(Event_Base *_event)
{
	//check if we even have a valid event
	if (_event == NULL) return;
	
	//tell the event who is sending it and which pipe it travels down
	_event->caller = host;
	_event->pipe = pipe;
	
	if (!ignoredelay && _event->delay == 0)
	{
		//the event must be generated immediately
		processEvent(_event);
	}
	else
	{
		//check if an event of same type is already present in the queue.
		//we don't need duplicate events in the queue, that's the main reason why
		//a delay exists: so in circumstances where an event would get triggered
		//several times in short order, it only gets triggered once when all is done
		for (UINT i = 0; i < eventqueue.size(); ++i)
		{
			if (*eventqueue[i] == _event)
			{
				delete _event;
				return;
			}
		}
		//add the event to the queue for later processing
		eventqueue.push_back(_event);
	}
}


/* goes through the event queue and executes the events that are due.
 * if an event requires longer delay, it will be re-added to the event queue instead of propagated
 */
void EventGenerator::ProcessEvents()
{
	UINT inqueue = eventqueue.size();
	for (UINT i = 0; i < inqueue; ++i)
	{
		//remove the front of the queue
		Event_Base* e = eventqueue.front();
		eventqueue.pop_front();
		if (ignoredelay || e->sendMe())
		{
			//if the event is due, propagate it
			processEvent(e);
		}
		else
		{
			//if not, enqueue it again
			eventqueue.push_back(e);
		}
	}
}


/* The event pipes are the usual lines of communication.
 * However, every now and then it may be necessary for an event
 * to jump pipes. The necessity for this has to be determined
 * by the ProcessEvent implementation of the respective EventHandler.
 * Then the event can be relayed to another eventgenerator using this method,
 * Which will propagate the event instantly through its pipe. This means
 * that the event will propagate through this entire pipe first, before
 * propagation through the original pipe is resumed.
 * RelayEvent will NOT delete the event even if it was consumed,
 * but it will return whether the event was consumed. If it was,
 * the relaying ProcessEvent implementation has to decide whether to
 * consume the event (recommended) itself or continue propagation through its own pipe. 
 */
 bool EventGenerator::RelayEvent(Event_Base *_event)
{
	//store the original event pipe, since it will have to be reset after the relay has finished
	EVENTPIPE original_event_pipe = _event->GetEventPipe();
	_event->pipe = pipe;
	bool consumed = EventNode::processEvent(_event);
	_event->pipe = original_event_pipe;
	return consumed;
}


bool EventGenerator::processEvent(Event_Base *e)
{
	//send the event to the superclass for propagation and delete it afterwards
	bool consumed = EventNode::processEvent(e);
	delete e;
	return consumed;
}

