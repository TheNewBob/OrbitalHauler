#include "core/Common.h"
#include "OpStdLibs.h"
#include "OpForwardDeclare.h"
#include "event/Events.h"

#include "systems/VesselSystem.h"
#include "model/ThrusterConfig.h"
#include "MainEngine.h"
#include "core/OrbitalHauler.h"


MainEngine::MainEngine(ThrusterConfig config, OrbitalHauler* vessel) : VesselSystem(vessel), config(config) {}
MainEngine::~MainEngine() {}


void MainEngine::init() {
	Olog::trace("Main engine init");

	// create event subscriptions
	EventBroker &broker = vessel->getEventBroker();
	broker.subscribe((EventSubscriber*)this, EVENTTOPIC::GENERAL);


	// Create the propellant tank.
	// TODO: If we're going to do anything complex with tanks, they should probably become an own VesselSystem instance.
	double propMass = 20000;
	PROPELLANT_HANDLE propHandle = vessel->CreatePropellantResource(propMass);
	
	// Create the thruster
	// TODO: Some of this should be configurable in the CFG.

	VECTOR3 pos = { 0, 0, -5 };
	VECTOR3 dir = { 0, 0, 1 };
	double thrust = config.thrust;
	double isp = config.isp;
	
	THRUSTER_HANDLE thrustHandle = vessel->CreateThruster(pos, dir, thrust, propHandle, isp);

	vessel->CreateThrusterGroup(&thrustHandle, 1, THGROUP_MAIN);
	vessel->AddExhaust(thrustHandle, 8, 1, pos, dir * -1);

}

void MainEngine::receiveEvent(Event_Base* event, EVENTTOPIC topic) {

	if (*event == EVENTTYPE::SIMULATIONSTARTEDEVENT) {
		Olog::info("Main engine received sim started event!");
	}
}