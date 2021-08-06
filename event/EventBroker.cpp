#include <windows.h>
#include "EventTypes.h"
#include "Event_Base.h"
#include <vector>
#include <map>
#include <deque>
#include "EventSubscriber.h"
#include "EventBroker.h"

using namespace std;

EventBroker::EventBroker() {};
EventBroker::~EventBroker() {
	// Walk through all event queues and delete all unsent events.
	for (const auto& it : eventqueues) {
		clearEventsForTopic(it.first);
	}
};

bool EventBroker::subscribe(EventSubscriber *subscriber, EVENTTOPIC topic) {
	vector<EventSubscriber*>& topicSubscribers = subscribers[topic];

	if (std::find(topicSubscribers.begin(), topicSubscribers.end(), subscriber) == topicSubscribers.end()) {
		// The subscriber is not subscribed to this topic yet.
		topicSubscribers.push_back(subscriber);
		return true;
	}
	
	// The subscriber is already subscribed to this topic, don't add it a second time!
	return false;
}

bool EventBroker::unsubscribe(EventSubscriber *subscriber, EVENTTOPIC topic) {
	vector<EventSubscriber*>& topicSubscribers = subscribers[topic];

	vector<EventSubscriber*>::iterator it = topicSubscribers.begin();
	if (std::find(it, topicSubscribers.end(), subscriber) != topicSubscribers.end()) {
		topicSubscribers.erase(it);
		if (topicSubscribers.size() == 0) {
			subscribers.erase(topic);
			clearEventsForTopic(topic);
			eventqueues.erase(topic);
		}
		return true;
	}
	return false;
}

void EventBroker::publish(EVENTTOPIC topic, Event_Base* event) {
	{
		//check if we even have a valid event
		if (event == NULL) return;

		//tell the event who is sending it and which pipe it travels down
		// TODO: caller transference wil be a bit more tricky here.
//		_event->caller = host;
//		_event->pipe = pipe;

		if (event->delay <= 0)
		{
			//the event must be propagated immediately
			propagateEvent(topic, event);
			delete event;
		}
		else
		{
			deque<Event_Base*>& topicQueue = eventqueues[topic];

			//check if the event is already present in the queue.
			//we don't need duplicate events in the queue, that's the main reason why
			//a delay exists: so in circumstances where events may trigger costly calculations
			//multiple times in the same frame, they only get triggered once in the next.
			for (UINT i = 0; i < topicQueue.size(); ++i)
			{
				if (*topicQueue[i] == event)
				{
					delete event;
					return;
				}
			}
			//add the event to the queue for later processing
			topicQueue.push_back(event);
		}
	}
}

/** 
 * \brief goes through the event queues and executes the events that are due.
 * if an event requires longer delay, it will be re-added to the event queue instead of propagated
 */
void EventBroker::processEvents() {

	for (map<EVENTTOPIC, deque<Event_Base*>>::iterator it = eventqueues.begin(); it != eventqueues.end(); it++)
	{
		deque<Event_Base*> &queue = it->second;
		UINT inqueue = queue.size();
		for (UINT i = 0; i < inqueue; ++i)
		{
			//remove the front of the queue
			Event_Base* e = queue.front();
			queue.pop_front();
			if (e->sendMe())
			{
				//if the event is due, propagate it, then delete it.
				propagateEvent(it->first, e);
				delete e;
			}
			else
			{
				//if not, enqueue it again
				queue.push_back(e);
			}
		}
	}

}

void EventBroker::relay(EVENTTOPIC topic, Event_Base* event) {
	propagateEvent(topic, event);
}

void EventBroker::propagateEvent(EVENTTOPIC topic, Event_Base *event) {
	vector<EventSubscriber*> &topicSubscribers = subscribers[topic];
	for (vector<EventSubscriber*>::iterator it = topicSubscribers.begin(); it != topicSubscribers.end(); it++) {
		(*it)->receiveEvent(event, topic);
	}
}

void EventBroker::clearEventsForTopic(EVENTTOPIC topic) {
	
	deque<Event_Base*>& topicQueue = eventqueues[topic];
	UINT inqueue = topicQueue.size();
	for (UINT i = 0; i < inqueue; ++i)
	{
		Event_Base* e = topicQueue.front();
		topicQueue.pop_front();
		delete e;
	}
}



