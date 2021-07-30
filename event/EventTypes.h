#pragma once

/**
 * \file EventTypes.h
 * Defines identifiers for event types, event pipes EventHandler types
 */

/**
 * \brief Used to quickly identify events without having to cast them
 * \note When writing a new event class, add a new identifer for it here.
 */
typedef enum
{
	DOCKEVENT,
	SHIFTCGEVENT,
	CGHASCHANGEDEVENT,
	VESSELLAYOUTCHANGEDEVENT,
	LIFTOFFEVENT,
	TOUCHDOWNEVENT,
	MASSHASCHANGEDEVENT,
	CONSUMABLEADDEDEVENT,
	RCSCHANGEDEVENT,
	SIMULATIONSTARTEDEVENT,
	HULLSHAPEREMOVEDEVENT,
	TDPOINTSCHANGEDEVENT,
	TOGGLEGEAREVENT,

	STARTANIMATIONEVENT,
	STOPANIMATIONEVENT,
	ANIMATIONFINISHEDEVENT,
	VESSELVISUALCREATEDEVENT,
	VESSELVISUALDESTROYEDEVENT,
	TRACKINGANIMATIONSTATUSEVENT,
	MODIFYANIMATIONEVENT,
	ANIMATIONSTARTEDEVENT,
	ANIMATIONFAILEDEVENT,
	SETANIMATIONSTATEEVENT
} EVENTTYPE;


/**
 * \brief Defines existing event pipes
 */
typedef enum
{
	WAITING_PIPE,					//!< The waiting pipe is a pipe that is treated special by event handlers. This pipe will not be processed on sendEvents(), but on processWaitingQueue() instead
	DEFAULT_PIPE,					//!< The default pipe depends on the settings of the respective EventHandler generating the event
	VESSEL_TO_SYSTEMS_PIPE,			//!< Carries events from the vessel to modules, and from there to module functions
} EVENTPIPE;


/**
 * \brief Defines the possible EventHandler types
 * \note EventHandler types in IMS2 signify the kind of object hiding behind the EventHandler
 */
typedef enum
{
	VESSEL_HANDLER,					
	SYSTEM_HANDLER,
} EVENTHANDLERTYPE;