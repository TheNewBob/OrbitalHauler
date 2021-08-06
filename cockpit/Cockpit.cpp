#include "core/Common.h"

#include "event/Events.h"
#include "systems/VesselSystem.h"
#include "Cockpit.h"
#include "core/OrbitalHauler.h"



void Cockpit::init(OrbitalHauler *vessel) {

	vessel->SetMeshVisibilityMode(vessel->AddMesh(mesh = oapiLoadMeshGlobal("ShuttleA\\ShuttleA_vc")), MESHVIS_VC);
	

}
