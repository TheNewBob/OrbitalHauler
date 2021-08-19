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
#include "event/events/MouseEvent.h"
#include "event/events/AnimationEvent_Base.h"
#include "event/events/AnimationStartedEvent.h"
#include "event/events/AnimationFinishedEvent.h"
#include "event/events/AnimationFailedEvent.h"
#include "event/events/StartAnimationEvent.h"
#include "event/events/StopAnimationEvent.h"
#include "event/events/StartTrackingAnimationEvent.h"
#include "event/events/ModifyAnimationEvent.h"
#include "event/events/ModifyTrackingAnimationEvent.h"
#include "event/events/TrackingAnimationStatusEvent.h"


