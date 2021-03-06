#pragma once

/**
 * \file Events.h
 * Central include header for event classes. Include this header to get access to all event functionality
 */

#include <queue>
#include "EventTypes.h"
#include "Event_Base.h"
#include "Event_Timed.h"
#include "EventSubscriber.h"
#include "EventBroker.h"

//include the different event classes
#include "event/events/SimpleEvent.h"

