#pragma once


class EventBroker {
public:
	EventBroker();
	~EventBroker();

	bool subscribe(EventSubscriber *subscriber, EVENTTOPIC topic);
	bool unsubscribe(EventSubscriber *subscriber, EVENTTOPIC topic);
	void publish(EVENTTOPIC topic, Event_Base* event);
	void relay(EVENTTOPIC topic, Event_Base* event);

	/**
	 * \brief Goes through the event queues, sends all events that are due, decrements counter on the rest.
	 * This method should be called *once* each frame.
	 */
	void processEvents();

private:
	void propagateEvent(EVENTTOPIC topic, Event_Base *event);
	
	std::map<EVENTTOPIC, std::vector<EventSubscriber*>> subscribers;

	std::map<EVENTTOPIC, std::deque<Event_Base*>> eventqueues;					//!< The eventqueue, containing all events that are waiting to be sent out.
};

