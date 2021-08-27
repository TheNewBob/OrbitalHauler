#include "core/Common.h"
#include "event/Events.h"
#include "OpForwardDeclare.h"

#include "animation/AnimationData.h"
#include "animation/Animation_Base.h"
#include "animation/AnimationManager.h"

#include "InstrumentPanelElement.h"

#include "util/Rotations.h"


InstrumentPanelElement::InstrumentPanelElement(VESSEL4* vessel, int vcAreaId):
	vessel(vessel), vcAreaId(vcAreaId) {}

InstrumentPanelElement::~InstrumentPanelElement() {}


void InstrumentPanelElement::init(VECTOR3 &absoluteElementPosition, EventBroker *eventBroker, EVENTTOPIC receiverTopic, MATRIX3& panelRotation, AnimationManager* animationManager) {

	this->eventBroker = eventBroker;
	this->receiverTopic = receiverTopic;

	// Add element mesh to the virtual cockpit
	meshIndex = vessel->GetMeshCount();

	// Let element implementations define the mesh and oter stuff, most commonly animations.
	initElement(panelRotation, absoluteElementPosition, animationManager);

	vessel->SetMeshVisibilityMode(meshIndex, MESHVIS_VC);

}

void InstrumentPanelElement::visualCreated(VISHANDLE vis, MATRIX3& panelRotation) {

	// Rotate the mesh to the panel
	DEVMESHHANDLE devMesh = vessel->GetDevMesh(vis, meshIndex);
	Rotations::TransformMesh(mesh, devMesh, panelRotation);
	// TODO: Scaling
}







