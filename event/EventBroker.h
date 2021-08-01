#pragma once

/**
 * A synchronous message broker for framelocked applications, implementing a pub/sub pattern.
 * 
 * The main goal is, as is typical for event engines, to decouple invocation and execution of code.
 * It is a frequent problem that some state changes in the simulation need to trigger some recalculation of other states.
 * If you this by direct invocation of a method, you're running the danger that another state change happens in the same frame 
 * that requires the same calculation again, so now you wasted a couple of cycles.
 * A way out of this is setting flags and processig them at the beginning of the next frame. If you have a lot of these flags, 
 * that'll get messy pretty quickly, and they need to be either put in a place where they are accessible to everyone, or you need
 * to write a lot of boilerplate to do all the necessary invocations when a centralised authority checks the flag.
 * 
 * This event engine aims to solve that problem by a pub/sub pattern, where every EventSubscriber instance receives the events for the topics they are subscribed to
 * in their receiveEvent() method, which is intended to be implemented by an inheriting class as an event handler.
 * 
 * Everyone with access to the broker can publish events to a topic. When receiving events, the broker checks for duplications, 
 * and does not add them to the event queue if a duplicate for the topic exists. In other words, if multiple agents publish the same event,
 * it will be propagated only once through the topic.
 * 
 * Events have a delay, which is typically 1. This means that the event will be propagated during the next call to processEvents(), 
 * which should be called once at the beginning of each frame. Events can have longer delays, meaning you can hold them back for multiple frames before they are propagated.
 * Events *can* have a delay of 0, which means they will be propagated *immediately* when they are published. This may be helpful in some rare cases, but should not be the norm!
 * 
 * You should instantiate events on the heap when publishing. The broker will take ownership of the published event, deleting it after its propagation,
 * so you don't have to worry about it.
 * 
 * At times, it may be necessary to make an event jump topics for some reason. In this case, you can use the brokers relay() method. 
 * A relayed event will be propagated *immediately*, and will *not* destroy it after propagation, so that propagation in the current topic isn't disturbed.
 * In other words, you should only ever use the relay() method for events received *from* an event broker. If you invoke relay with an event that is not comming
 * from the broker, the overall effect will be *the exact same* as regularly publishing an event with a delay of 0, *except* that you'd have to clean up
 * the event itself or you'd be creating a memory leak. Needless to say, I strongly discourage any such use of the relay method.
 * 
 */
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
	 * This method should be called *once* at the beginning of each frame.
	 */
	void processEvents();

private:
	void propagateEvent(EVENTTOPIC topic, Event_Base *event);
	
	std::map<EVENTTOPIC, std::vector<EventSubscriber*>> subscribers;

	std::map<EVENTTOPIC, std::deque<Event_Base*>> eventqueues;					//!< The eventqueue, containing all events that are waiting to be sent out.
};

