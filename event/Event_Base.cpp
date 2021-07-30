#include "core/Common.h"
#include "EventTypes.h"
#include "Event_Base.h"


Event_Base::Event_Base(EVENTTYPE _type, unsigned int _delay)
	: eventtype(_type), delay(_delay)
{
}


Event_Base::~Event_Base()
{
}

bool Event_Base::sendMe()
{
	Olog::assertThat([this]() { return delay != UINT_MAX; }, "Are you SURE this delay is correct? Because me thinks that it went below zero without being sent!");
	delay -= 1;
	if (delay == 0)
	{
		return true;
	}
	return false;
}

bool Event_Base::operator==(Event_Base *e)
{
	//if the types of the event are identical, they are considered the same event
	if (eventtype == e->eventtype)
	{
		return true;
	}
	return false;
}

bool Event_Base::operator==(EVENTTYPE type)
{
	return eventtype == type;
}

bool Event_Base::operator!=(Event_Base *e)
{
	return !(this == e);
}
