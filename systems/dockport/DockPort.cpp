#include "core/Common.h"
#include "event/Events.h"
#include "systems/VesselSystem.h"

#include "DockPort.h"
#include "core/OrbitalHauler.h"



DockPort::DockPort(OrbitalHauler* vessel) : VesselSystem(vessel) {}
DockPort::~DockPort() {}

void DockPort::init() {
	
	// create event subscriptions
	EventBroker &broker = vessel->getEventBroker();
	broker.subscribe((EventSubscriber*)this, EVENTTOPIC::GENERAL);


	VECTOR3 pos = { 0, 0, 5 };
	VECTOR3 rot = { 0, 1, 0 };
	VECTOR3 dir = { 0, 0, 1 };

	vessel->CreateDock(pos, dir, rot);
}


void DockPort::receiveEvent(Event_Base* event, EVENTTOPIC topic) {

	if (*event == EVENTTYPE::SIMULATIONSTARTEDEVENT) {
		Olog::info("Dockport received sim started event!");
	}
}

