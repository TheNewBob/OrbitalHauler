#include "core/Common.h"
#include "event/Events.h"
#include "InstrumentPanelElement.h"
#include "InstrumentPanel.h"

#include "util/Calc.h"
#include "util/Rotations.h"


InstrumentPanel::InstrumentPanel(VESSEL4* vessel): vessel(vessel) {}

InstrumentPanel::~InstrumentPanel() {
	for (const auto& it : elements) {
		delete it;
	}
}

void InstrumentPanel::init() {
	Olog::assertThat([&]() { return isInitialised == false; }, "Please don't initialise a panel twice, it's a mess you don't want!");
	
	// TODO: Make the thing placeable in the cockpit!
	position = _V(0, 0, 1);
	rotation = _V(0, 0, -1);
	direction = _V(0, 1, 0);
	scale = _V(2, 1, 2);

	rotationMatrix = Rotations::GetRotationMatrixFromOrientation(direction, rotation);
	
	// TODO: I suspect this is what messes up rotating switches into place right now, but it's not fixed with a simple mul. Need to think this through.
	VECTOR3 upperLeftCorner = _V(position.x - scale.x / 2, position.y, position.z + scale.z / 2);
	for (const auto& it : elements) {
		it->init(upperLeftCorner, rotationMatrix);
	}

	if (drawBackground) {
		mesh = oapiLoadMeshGlobal("switchflip\\plane");
		meshIndex = vessel->AddMesh(mesh, &position);
		vessel->SetMeshVisibilityMode(meshIndex, MESHVIS_VC);
	}

	isInitialised = true;
}

void InstrumentPanel::loadVc() {
	Olog::assertThat([&]() { return isInitialised == true; }, "Panel must be initialised before calling loadVc!");

	for (const auto& it : elements) {
		it->loadVc();
	}
}

void InstrumentPanel::visualCreated(VISHANDLE vis) {

	// Rotate the mesh to the set rotation
	if (drawBackground && Rotations::RequiresRotation(direction, rotation))
	{

		//MESHHANDLE meshTemplate = vessel->GetMeshTemplate(meshIndex);
		DEVMESHHANDLE devMesh = vessel->GetDevMesh(vis, meshIndex);

		Rotations::TransformMesh(mesh, devMesh, rotationMatrix);
	}

	for (const auto& it : elements) {
		it->visualCreated(vis, rotationMatrix);
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



