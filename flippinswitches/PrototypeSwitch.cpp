#include "core/Common.h"
#include "event/Events.h"
#include "InstrumentPanelElement.h"
#include "PrototypeSwitch.h"

#include "util/Rotations.h"

PrototypeSwitch::PrototypeSwitch(VESSEL4* vessel, EventBroker& eventBroker, EVENTTOPIC receiverTopic, int vcAreaId, VECTOR3 position)
	: InstrumentPanelElement(vessel, eventBroker, receiverTopic, vcAreaId, position) {}

PrototypeSwitch::~PrototypeSwitch() {
	delete meshGroups;
	delete animComponent;
}

void PrototypeSwitch::init(VECTOR3& panelPosition, MATRIX3& panelRotation) {
	
	// TODO: a bit ugly, the panel should place the element, not the element itself...
	// Rotate the switch's position relative to the panel.
	position = mul(panelRotation, position);
	// Transform the switch's position from panel-relative to vessel-relative coordinates.
	position = _V(panelPosition.x + position.x, panelPosition.y + position.y, panelPosition.z - position.z);
	// Rotate switch to panel
	meshIndex = vessel->GetMeshCount();
	vessel->SetMeshVisibilityMode(vessel->AddMesh(mesh = oapiLoadMeshGlobal("switchflip\\switch"), &position), MESHVIS_VC);

	meshGroups = new UINT[1];
	meshGroups[0] = 0;
	
	VECTOR3 rotationReference = mul(panelRotation, _V(-0, 0, 0));
	VECTOR3 rotationAxis = mul(panelRotation, _V(0, 0, 1));

	animComponent = new MGROUP_ROTATE(meshIndex, meshGroups, 1, rotationReference, rotationAxis, 0.35);
	animationId = vessel->CreateAnimation(0.0);
	vessel->AddAnimationComponent(animationId, 0.0f, 1.0f, animComponent);

}

void PrototypeSwitch::loadVc() {
	// This stuff *needs* to be called from clbkLoadVC, or it won't have an effect. Almost went crazy...
	oapiVCRegisterArea(vcAreaId, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
	oapiVCSetAreaClickmode_Spherical(vcAreaId, position, 0.1);

}

void PrototypeSwitch::toggle() {

	if (state) {
		vessel->SetAnimation(animationId, 0);
		state = false;
	}
	else {
		vessel->SetAnimation(animationId, 1);
		state = true;
	}

}

bool PrototypeSwitch::processMouseEvent(int event, VECTOR3& position) {
	if (event == PANEL_MOUSE_LBDOWN) {
		toggle();
	}
	return true;
}

void PrototypeSwitch::visualCreated(VISHANDLE vis, MATRIX3& panelRotation) {
	// Rotate the mesh to the panel
	DEVMESHHANDLE devMesh = vessel->GetDevMesh(vis, meshIndex);
	Rotations::TransformMesh(mesh, devMesh, panelRotation);
}