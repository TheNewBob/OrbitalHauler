#pragma once

class EventSubscriber
{
	friend class EventBroker;
protected:
	virtual void receiveEvent(Event_Base* event, EVENTTOPIC topic) = 0;
};

