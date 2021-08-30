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
	: InstrumentPanelElement(vessel, vcAreaId), EventSubscriber() {}

PrototypeSwitch::~PrototypeSwitch() {
	for (const auto& it : animationData->components) {
		delete it;
	}
	delete animationData;
}

void PrototypeSwitch::initElement(MATRIX3 &panelRotation, VECTOR3& elementAbsolutePosition, AnimationManager* animationManager) {
	
	mesh = oapiLoadMeshGlobal("switchflip/switch");
	UINT meshIdx = vessel->AddMesh(mesh, &elementAbsolutePosition);
	animationId = "PrototypeSwitch" + to_string(meshIdx);

	// TODO: testing hook-up, properties should be moved to a config file for the switch, but I have to add the animation manager first...

	
	ANIMCOMPONENTDATA* rotation = new ANIMCOMPONENTDATA();
	rotation->axis = _V(0, 0, 1);
	rotation->reference = _V(0, 0, 0);
	rotation->type = "rotate";
	rotation->range = 20;
	rotation->groups.push_back(0);

	animationData = new ANIMATIONDATA();
	animationData->components.push_back(rotation);
	animationData->duration = 0.05;
	animationData->type = "sequence";
	animationData->rotationMatrix = panelRotation;
	animationData->id = animationId;
	animationData->meshIndex = meshIdx;

	animationManager->addAnimation(animationData);

	eventBroker->subscribe(this, EVENTTOPIC::ANIMATION);
}

void PrototypeSwitch::loadVc(VECTOR3& elementAbsolutePosition) {
	// This stuff *needs* to be called from clbkLoadVC, or it won't have an effect. Almost went crazy...
	oapiVCRegisterArea(vcAreaId, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
	oapiVCSetAreaClickmode_Spherical(vcAreaId, elementAbsolutePosition, 0.1);

}

void PrototypeSwitch::toggleSwitch() {

	if (state) {
		eventBroker->publish(EVENTTOPIC::ANIMATION, new StartAnimationEvent(animationId, -1));
	}
	else {
		eventBroker->publish(EVENTTOPIC::ANIMATION, new StartAnimationEvent(animationId, 1));
	}

}

bool PrototypeSwitch::processMouseEvent(int event, VECTOR3& position) {
	if (event == PANEL_MOUSE_LBDOWN) {
		toggleSwitch();
	}
	return true;
}

void PrototypeSwitch::receiveEvent(Event_Base* event, EVENTTOPIC topic) {

	if (topic == EVENTTOPIC::ANIMATION) {
		if (*event == EVENTTYPE::ANIMATIONFINISHEDEVENT) {
			AnimationFinishedEvent* finishedEvent = (AnimationFinishedEvent*)event;
			if (finishedEvent->GetAnimationId() == animationData->id) {
				if (finishedEvent->getState() > 0.999) {
					state = true;
				}
				else {
					state = false;
				}
			}
		}
	}
}

