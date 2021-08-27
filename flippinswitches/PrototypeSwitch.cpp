#include "core/Common.h"
#include "event/Events.h"
#include "OpForwardDeclare.h"

#include "animation/AnimationData.h"
#include "animation/Animation_Base.h"
#include "animation/Animation_Sequential.h"
#include "animation/AnimationManager.h"

#include "InstrumentPanelElement.h"
#include "PrototypeSwitch.h"
#include "util/Rotations.h"

PrototypeSwitch::PrototypeSwitch(VESSEL4* vessel, int vcAreaId)
	: InstrumentPanelElement(vessel, vcAreaId) {}

PrototypeSwitch::~PrototypeSwitch() {
	delete meshGroups;
	delete animComponent;
}

void PrototypeSwitch::initElement(MATRIX3 &panelRotation, VECTOR3& elementAbsolutePosition, AnimationManager* animationManager) {
	
	mesh = oapiLoadMeshGlobal("switchflip/switch");
	UINT meshIdx = vessel->AddMesh(mesh, &elementAbsolutePosition);
	animationId = "PrototypeSwitch" + to_string(meshIdx);

	meshGroups = new UINT[1];
	meshGroups[0] = 0;

	
	// TODO: testing hook-up, properties should be moved to a config file for the switch, but I have to add the animation manager first...

	
	rotation = new ANIMCOMPONENTDATA();
	rotation->axis = _V(0, 0, 1);
	rotation->reference = _V(0, 0, 0);
	rotation->type = "rotate";
	rotation->range = 20;

	animationData = new ANIMATIONDATA();
	animationData->components.push_back(rotation);
	animationData->duration = 5;
	animationData->type = "sequence";
	animationData->rotationMatrix = panelRotation;
	animationData->id = animationId;

	animationManager->addAnimation(animationData);
	//	animation = new Animation_Sequential(animationData);
//	animation->AddAnimationToVessel(vessel, meshIndex, panelRotation, elementAbsolutePosition);
	

	// Rotate animation references to panel.

//	VECTOR3 rotationReference = mul(panelRotation, _V(-0, 0, 0));
//	VECTOR3 rotationAxis = mul(panelRotation, _V(0, 0, 1));

//	animComponent = new MGROUP_ROTATE(meshIndex, meshGroups, 1, rotationReference, rotationAxis, 0.35);
//	animationId = vessel->CreateAnimation(0.0);
//	vessel->AddAnimationComponent(animationId, 0.0f, 1.0f, animComponent);

}

void PrototypeSwitch::loadVc(VECTOR3& elementAbsolutePosition) {
	// This stuff *needs* to be called from clbkLoadVC, or it won't have an effect. Almost went crazy...
	oapiVCRegisterArea(vcAreaId, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
	oapiVCSetAreaClickmode_Spherical(vcAreaId, elementAbsolutePosition, 0.1);

}

void PrototypeSwitch::toggle() {

	if (state) {
		eventBroker->publish(EVENTTOPIC::ANIMATION, new StartAnimationEvent(animationId, -1));
	}
	else {
		eventBroker->publish(EVENTTOPIC::ANIMATION, new StartAnimationEvent(animationId, 1));
	}

}

bool PrototypeSwitch::processMouseEvent(int event, VECTOR3& position) {
	if (event == PANEL_MOUSE_LBDOWN) {
		toggle();
	}
	return true;
}

