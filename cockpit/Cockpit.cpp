#include "core/Common.h"

#include "event/Events.h"
#include "systems/VesselSystem.h"
#include "Cockpit.h"



void Cockpit::init(VESSEL4 *vessel) {
	VECTOR3 paneloffset = _V(0, 0, 0);
	MESHHANDLE panelMesh = oapiLoadMeshGlobal("switchflip\\panel");
	vessel->SetMeshVisibilityMode(vessel->AddMesh(panelMesh , &paneloffset), MESHVIS_VC);

	VECTOR3 offset = _V(0, 0.05, 0);
	vessel->SetMeshVisibilityMode(vessel->AddMesh(mesh = oapiLoadMeshGlobal("switchflip\\switch"), &offset), MESHVIS_VC);
}

void Cockpit::defineAreas() {
	// This stuff *needs* to be called from clbkLoadVC, or it won't have an effect. Almost went crazy...
	oapiVCRegisterArea(1, PANEL_REDRAW_ALWAYS, PANEL_MOUSE_LBDOWN);

	oapiVCSetAreaClickmode_Spherical(1, _V(0, 0, 0), 0.5);
}
