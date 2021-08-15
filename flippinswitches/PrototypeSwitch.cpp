#include "core/Common.h"
#include "event/Events.h"
#include "InstrumentPanelElement.h"
#include "PrototypeSwitch.h"

#include "util/Rotations.h"

PrototypeSwitch::PrototypeSwitch(VESSEL4* vessel, int vcAreaId)
	: InstrumentPanelElement(vessel, vcAreaId) {}

PrototypeSwitch::~PrototypeSwitch() {
	delete meshGroups;
	delete animComponent;
}

void PrototypeSwitch::initElement(MATRIX3 &panelRotation) {
	

	meshGroups = new UINT[1];
	meshGroups[0] = 0;

	// Rotate animation references to panel.
	VECTOR3 rotationReference = mul(panelRotation, _V(-0, 0, 0));
	VECTOR3 rotationAxis = mul(panelRotation, _V(0, 0, 1));

	animComponent = new MGROUP_ROTATE(meshIndex, meshGroups, 1, rotationReference, rotationAxis, 0.35);
	animationId = vessel->CreateAnimation(0.0);
	vessel->AddAnimationComponent(animationId, 0.0f, 1.0f, animComponent);

}

void PrototypeSwitch::loadVc(VECTOR3& elementAbsolutePosition) {
	// This stuff *needs* to be called from clbkLoadVC, or it won't have an effect. Almost went crazy...
	oapiVCRegisterArea(vcAreaId, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
	oapiVCSetAreaClickmode_Spherical(vcAreaId, elementAbsolutePosition, 0.1);

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

