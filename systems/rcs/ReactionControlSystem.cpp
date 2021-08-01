#include "core/Common.h"
#include "OpStdLibs.h"
#include "OpForwardDeclare.h"
#include "event/Events.h"

#include "systems/VesselSystem.h"
#include "model/ThrusterConfig.h"
#include "systems/rcs/ReactionControlSystem.h"

#include "core/OrbitalHauler.h"


ReactionControlSystem::ReactionControlSystem(ThrusterConfig config, OrbitalHauler *vessel) : VesselSystem(vessel), config(config) {}
ReactionControlSystem::~ReactionControlSystem() {}

void ReactionControlSystem::init(EventBroker& eventBroker) {
	Olog::trace("RCS init");

	// create event subscriptions
	eventBroker.subscribe((EventSubscriber*)this, EVENTTOPIC::GENERAL);


	// Create the propellant tank.
	double propMass = 500;
	PROPELLANT_HANDLE propHandle = vessel->CreatePropellantResource(propMass);

	// Create the RCS thrusters. 
	// TODO: Arrange RCS thrusters into some smarter data structure so their creation can be written a bit more readable.
	// This messy stuff is copied and modified from ShuttlePB because I'm lazy and they're not going to stay anyways. 
	THRUSTER_HANDLE th_rcs[14], th_group[4];
	double thrust = config.thrust;
	double isp = config.isp;

	th_rcs[0] = vessel->CreateThruster(_V(1, 0, 5), _V(0, 1, 0), thrust, propHandle, isp);
	th_rcs[1] = vessel->CreateThruster(_V(1, 0, 5), _V(0, -1, 0), thrust, propHandle, isp);
	th_rcs[2] = vessel->CreateThruster(_V(-1, 0, 5), _V(0, 1, 0), thrust, propHandle, isp);
	th_rcs[3] = vessel->CreateThruster(_V(-1, 0, 5), _V(0, -1, 0), thrust, propHandle, isp);
	th_rcs[4] = vessel->CreateThruster(_V(1, 0, -5), _V(0, 1, 0), thrust, propHandle, isp);
	th_rcs[5] = vessel->CreateThruster(_V(1, 0, -5), _V(0, -1, 0), thrust, propHandle, isp);
	th_rcs[6] = vessel->CreateThruster(_V(-1, 0, -5), _V(0, 1, 0), thrust, propHandle, isp);
	th_rcs[7] = vessel->CreateThruster(_V(-1, 0, -5), _V(0, -1, 0), thrust, propHandle, isp);
	th_rcs[8] = vessel->CreateThruster(_V(1, 0, 5), _V(-1, 0, 0), thrust, propHandle, isp);
	th_rcs[9] = vessel->CreateThruster(_V(-1, 0, 5), _V(1, 0, 0), thrust, propHandle, isp);
	th_rcs[10] = vessel->CreateThruster(_V(1, 0, -5), _V(-1, 0, 0), thrust, propHandle, isp);
	th_rcs[11] = vessel->CreateThruster(_V(-1, 0, -5), _V(1, 0, 0), thrust, propHandle, isp);
	th_rcs[12] = vessel->CreateThruster(_V(0, 0, -5), _V(0, 0, 1), thrust, propHandle, isp);
	th_rcs[13] = vessel->CreateThruster(_V(0, 0, 5), _V(0, 0, -1), thrust, propHandle, isp);

	th_group[0] = th_rcs[0];
	th_group[1] = th_rcs[2];
	th_group[2] = th_rcs[5];
	th_group[3] = th_rcs[7];
	vessel->CreateThrusterGroup(th_group, 4, THGROUP_ATT_PITCHUP);

	th_group[0] = th_rcs[1];
	th_group[1] = th_rcs[3];
	th_group[2] = th_rcs[4];
	th_group[3] = th_rcs[6];
	vessel->CreateThrusterGroup(th_group, 4, THGROUP_ATT_PITCHDOWN);

	th_group[0] = th_rcs[0];
	th_group[1] = th_rcs[4];
	th_group[2] = th_rcs[3];
	th_group[3] = th_rcs[7];
	vessel->CreateThrusterGroup(th_group, 4, THGROUP_ATT_BANKLEFT);

	th_group[0] = th_rcs[1];
	th_group[1] = th_rcs[5];
	th_group[2] = th_rcs[2];
	th_group[3] = th_rcs[6];
	vessel->CreateThrusterGroup(th_group, 4, THGROUP_ATT_BANKRIGHT);

	th_group[0] = th_rcs[0];
	th_group[1] = th_rcs[4];
	th_group[2] = th_rcs[2];
	th_group[3] = th_rcs[6];
	vessel->CreateThrusterGroup(th_group, 4, THGROUP_ATT_UP);

	th_group[0] = th_rcs[1];
	th_group[1] = th_rcs[5];
	th_group[2] = th_rcs[3];
	th_group[3] = th_rcs[7];
	vessel->CreateThrusterGroup(th_group, 4, THGROUP_ATT_DOWN);

	th_group[0] = th_rcs[8];
	th_group[1] = th_rcs[11];
	vessel->CreateThrusterGroup(th_group, 2, THGROUP_ATT_YAWLEFT);

	th_group[0] = th_rcs[9];
	th_group[1] = th_rcs[10];
	vessel->CreateThrusterGroup(th_group, 2, THGROUP_ATT_YAWRIGHT);

	th_group[0] = th_rcs[8];
	th_group[1] = th_rcs[10];
	vessel->CreateThrusterGroup(th_group, 2, THGROUP_ATT_LEFT);

	th_group[0] = th_rcs[9];
	th_group[1] = th_rcs[11];
	vessel->CreateThrusterGroup(th_group, 2, THGROUP_ATT_RIGHT);

	vessel->CreateThrusterGroup(th_rcs + 12, 1, THGROUP_ATT_FORWARD);
	vessel->CreateThrusterGroup(th_rcs + 13, 1, THGROUP_ATT_BACK);
}


void ReactionControlSystem::receiveEvent(Event_Base* event, EVENTTOPIC topic) {
	
	if (*event == EVENTTYPE::SIMULATIONSTARTEDEVENT) {
		Olog::info("RCS received sim started event!");
	}
}
