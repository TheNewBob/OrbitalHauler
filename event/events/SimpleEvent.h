#pragma once

class SimpleEvent :
    public Event_Base
{
public:

    SimpleEvent(EVENTTYPE eventType, int delay = 1) : Event_Base(eventType, delay) {};
    ~SimpleEvent() {};
};

