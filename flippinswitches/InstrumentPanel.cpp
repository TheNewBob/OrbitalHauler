#include "core/Common.h"
#include "event/Events.h"
#include "InstrumentPanelElement.h"
#include "InstrumentPanel.h"


InstrumentPanel::InstrumentPanel(VESSEL4* vessel): vessel(vessel) {}

InstrumentPanel::~InstrumentPanel() {
	for (const auto& it : elements) {
		delete it;
	}
}

void InstrumentPanel::init() {
	Olog::assertThat([&]() { return isInitialised == false; }, "Please don't initialise a panel twice, it's a mess you don't want!");
	
	// TODO: Make the thing placeable in the cockpit!
	position = _V(0, 0, 0);
	rotation = _V(0, 0, 1);
	scale = _V(1, 1, 1);
	
	if (drawBackground) {
		mesh = oapiLoadMeshGlobal("switchflip\\switch");
		vessel->SetMeshVisibilityMode(vessel->AddMesh(mesh, &position), MESHVIS_VC);
	}
}

void InstrumentPanel::loadVc() {
	Olog::assertThat([&]() { return isInitialised == true; }, "Panel must be initialised before calling loadVc!");
	for (const auto& it : elements) {
		it->loadVc();
	}
}


void InstrumentPanel::addElement(InstrumentPanelElement* element) {
	Olog::assertThat([&]() { return isInitialised == false; }, "Cannot add elements to panel after it is initialised!");
	elements.push_back(element);
}

bool InstrumentPanel::processMouseEvent(int areaId, int event, VECTOR3& position) {
	for (const auto& it : elements) {
		if (it->getVcAreaId() == areaId) {
			return it->processMouseEvent(event, position);
		}
	}
	return false;
}



