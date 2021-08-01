#pragma once

class EventSubscriber
{
	friend class EventBroker;
protected:
	virtual void receiveEvent(Event_Base* _event) = 0;
};

