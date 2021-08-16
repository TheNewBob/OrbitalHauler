#include "core/Common.h"
#include "event/Events.h"
#include "InstrumentPanelElement.h"
#include "InstrumentPanel.h"

#include "util/Calc.h"
#include "util/Rotations.h"


InstrumentPanel::InstrumentPanel(VESSEL4* vessel): vessel(vessel) {
	// TODO: Make these configurable!
	position = _V(0, 0, 1);
	rotation = _V(0, 0, -1);
	direction = _V(0, 1, 0);
	scale = _V(2, 1, 2);

}

InstrumentPanel::~InstrumentPanel() {
	for (const auto& it : elements) {
		delete it;
	}
}

void InstrumentPanel::init(EventBroker* eventBroker, EVENTTOPIC receiverTopic) {
	Olog::assertThat([&]() { return isInitialised == false; }, "Please don't initialise a panel twice, it's a mess you don't want!");
	
	rotationMatrix = Rotations::GetRotationMatrixFromOrientation(direction, rotation);
	
	for (const auto& it : elements) {
		VECTOR3 elementAbsolutePosition = calculateElementsAbsolutePosition(it, rotationMatrix);
		it->init(elementAbsolutePosition, eventBroker, receiverTopic, rotationMatrix);
	}

	if (drawBackground) {
		mesh = oapiLoadMeshGlobal("switchflip\\plane");
		meshIndex = vessel->AddMesh(mesh, &position);
		vessel->SetMeshVisibilityMode(meshIndex, MESHVIS_VC);
	}

	isInitialised = true;
}

VECTOR3 InstrumentPanel::calculateElementsAbsolutePosition(InstrumentPanelElement* element, MATRIX3& panelRotation) {
	VECTOR3& naturalRelativePosition = element->getPanelRelativePosition();
	VECTOR3 rotatedPanelRelativePosition = mul(panelRotation, naturalRelativePosition);
	return position + rotatedPanelRelativePosition;

}

void InstrumentPanel::loadVc() {
	Olog::assertThat([&]() { return isInitialised == true; }, "Panel must be initialised before calling loadVc!");
	
	for (const auto& it : elements) {
		VECTOR3 elementPosition = calculateElementsAbsolutePosition(it, rotationMatrix);
		it->loadVc(elementPosition);
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


void InstrumentPanel::addElement(InstrumentPanelElement* element, double x, double y) {
	Olog::assertThat([&]() { return isInitialised == false; }, "Cannot add elements to panel after it is initialised!");
	// Calculate the elements position relative to the panels center
	double panelLeftEdge = scale.x / 2 * -1;
	double panelBottomEdge = scale.z / 2 * -1;

	VECTOR3 elementPosition = _V(panelLeftEdge + x, 0, panelBottomEdge + y);
	element->setPanelRelativePosition(elementPosition);
	
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



