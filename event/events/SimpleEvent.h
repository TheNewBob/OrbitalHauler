#pragma once

/**
 * \brief A simple event that expresses that something happened without further data attached.
 * All SimpleEvents of the same type in the same topic will be treated as duplicates by the broker.
 */
class SimpleEvent :
    public Event_Base
{
public:

    SimpleEvent(EVENTTYPE eventType, int delay = 1) : Event_Base(eventType, delay) {};
    ~SimpleEvent() {};
};

