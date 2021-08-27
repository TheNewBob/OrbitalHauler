#pragma once

/**
 * \file EventTypes.h
 * Defines identifiers for event types, event pipes EventHandler types
 */

/**
 * \brief Used to quickly identify events.
 * \note If you need a new event type, just put it here!
 */
enum class EVENTTYPE {
	SIMULATIONSTARTEDEVENT,
	MOUSEEVENT,
	
	// Animations
	ANIMATIONSTARTEDEVENT,
	ANIMATIONFAILEDEVENT,
	ANIMATIONFINISHEDEVENT,
	STARTANIMATIONEVENT,
	STOPANIMATIONEVENT,
	MODIFYANIMATIONEVENT,
	TRACKINGANIMATIONSTATUSEVENT,

};


/**
 * \brief Topic identifiers.
 * \note If you need a new topic, just put it here!
 */
enum class EVENTTOPIC
{
	GENERAL,
	UI_VC,
	ANIMATION

};
