#include "core/Common.h"
#include "event/Events.h"
#include "InstrumentPanelElement.h"

#include "util/Rotations.h"


InstrumentPanelElement::InstrumentPanelElement(VESSEL4* vessel, int vcAreaId):
	vessel(vessel), vcAreaId(vcAreaId) {}

InstrumentPanelElement::~InstrumentPanelElement() {}


void InstrumentPanelElement::init(VECTOR3 &absoluteElementPosition, EventBroker *eventBroker, EVENTTOPIC receiverTopic, MATRIX3& panelRotation) {

	// Add element mesh to the virtual cockpit
	meshIndex = vessel->GetMeshCount();
	vessel->SetMeshVisibilityMode(vessel->AddMesh(mesh = oapiLoadMeshGlobal((char*)meshName.c_str()), &absoluteElementPosition), MESHVIS_VC);

	// Let element implementations do additional things like defining animations.
	initElement(panelRotation);
}

void InstrumentPanelElement::visualCreated(VISHANDLE vis, MATRIX3& panelRotation) {

	// Rotate the mesh to the panel
	DEVMESHHANDLE devMesh = vessel->GetDevMesh(vis, meshIndex);
	Rotations::TransformMesh(mesh, devMesh, panelRotation);
	// TODO: Scaling
}







