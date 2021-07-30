#pragma once

/**
 * \file Events.h
 * Central include header for event classes. Include this header to get access to all event functionality
 */

#include <queue>
#include "EventTypes.h"
#include "Event_Base.h"
#include "Event_Timed.h"
#include "EventNode.h"
#include "EventSink.h"
#include "EventGenerator.h"
#include "EventHandler.h"

//include the different event classes
#include "event/events/SimulationStartedEvent.h"

