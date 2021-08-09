#include "core/Common.h"
#include "event/Events.h"
#include "PrototypeSwitch.h"

PrototypeSwitch::PrototypeSwitch(int panelId, VECTOR3 position): panelId(panelId), position(position) {}

PrototypeSwitch::~PrototypeSwitch() {
	oapiDeleteMesh(mesh);
	delete meshGroups;
	delete animComponent;
}

void PrototypeSwitch::init(VESSEL4* vessel, EventBroker &eventBroker) {
	this->vessel = vessel;
	eventBroker.subscribe(this, EVENTTOPIC::UI_VC);

	meshIndex = vessel->GetMeshCount();
	vessel->SetMeshVisibilityMode(vessel->AddMesh(mesh = oapiLoadMeshGlobal("switchflip\\switch"), &position), MESHVIS_VC);

	meshGroups = new UINT[1];
	meshGroups[0] = 0;
	
	animComponent = new MGROUP_ROTATE(meshIndex, meshGroups, 1, _V(-0, 0, 0), _V(0, 0, 1), 0.35);
	animationId = vessel->CreateAnimation(0.0);
	vessel->AddAnimationComponent(animationId, 0.0f, 1.0f, animComponent);

}

void PrototypeSwitch::loadVC() {
	// This stuff *needs* to be called from clbkLoadVC, or it won't have an effect. Almost went crazy...
	oapiVCRegisterArea(panelId, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
	oapiVCSetAreaClickmode_Spherical(panelId, position, 0.1);

}

void PrototypeSwitch::receiveEvent(Event_Base *event, EVENTTOPIC topic) {

	if (*event == EVENTTYPE::MOUSEEVENT) {
		MouseEvent* mouseEvent = (MouseEvent*)event;
		if (mouseEvent->getVcId() == panelId && mouseEvent->getEvent() == PANEL_MOUSE_LBDOWN) {
				toggle();
		}
	}
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