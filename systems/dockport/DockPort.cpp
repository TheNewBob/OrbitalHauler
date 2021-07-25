#include "core/Common.h"
#include "systems/VesselSystem.h"
#include "DockPort.h"
#include "core/OrbitalHauler.h"



DockPort::DockPort(OrbitalHauler* vessel) : VesselSystem(vessel) {}
DockPort::~DockPort() {}

void DockPort::init() {

	VECTOR3 pos = { 0, 0, 5 };
	VECTOR3 rot = { 0, 1, 0 };
	VECTOR3 dir = { 0, 0, 1 };

	vessel->CreateDock(pos, dir, rot);
}

