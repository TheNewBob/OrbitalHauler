
#define STRICT
#define ORBITER_MODULE

#include "core/Common.h"
#include "OpStdLibs.h"
#include "Oparse.h"
#include "model/Models.h"
#include "event/Events.h"

#include "systems/VesselSystem.h"
#include "systems/mainengine/MainEngine.h"
#include "systems/rcs/ReactionControlSystem.h"
#include "systems/dockport/DockPort.h"
#include "cockpit/Cockpit.h"
#include "switchflip/PrototypeSwitch.h"
#include "core/OrbitalHauler.h"


using namespace Oparse;



// Module Init and cleanup
DLLCLBK VESSEL* ovcInit(OBJHANDLE hvessel, int flightmodel)
{
	return new OrbitalHauler(hvessel, flightmodel);
}

DLLCLBK void ovcExit(VESSEL* vessel)
{
	if (vessel) delete (OrbitalHauler*)vessel;
}

DLLCLBK void InitModule(HINSTANCE hModule) {
	// Set default log level here.
	Olog::loglevel = OLOG_INFO;
	Olog::assertlevel = OLOG_DEBUG;
	Olog::projectName = "Orbital Hauler";
	Olog::debug("Initialising Module.");
}


// Vessel class

OrbitalHauler::OrbitalHauler(OBJHANDLE hVessel, int flightmodel) : VESSEL4(hVessel, flightmodel) { 

}

OrbitalHauler::~OrbitalHauler() {
	
	// Delete vessel systems
	for (const auto& it : systems) {
		delete it;
	}

}

void OrbitalHauler::clbkSetClassCaps(FILEHANDLE cfg) {

	Olog::setLogLevelFromFile(cfg);

	// Load vessel config
	OrbitalHaulerConfig config = OrbitalHaulerConfig();
	OpModelDef modelDef = config.GetModelDef();

	PARSINGRESULT result = ParseFile(cfg, modelDef, "OrbitalHauler.cfg");
	if (result.HasErrors()) {
		Olog::error((char*)result.GetFormattedErrorsForFile().c_str());
		throw std::runtime_error("Errors in OrbitalHauler config, see log for details!");
	}

	// create vessel systems
	systems.push_back(new MainEngine(config.mainEngineConfig, this));
	systems.push_back(new ReactionControlSystem(config.rcsConfig, this));
	systems.push_back(new DockPort(this));

	// Initialise vessel systems

	for (const auto& it : systems) {
		it->init(eventBroker);
	}


	// VC (experimental)
	VECTOR3 paneloffset = _V(0, 0, 0);
	MESHHANDLE panelMesh = oapiLoadMeshGlobal("switchflip\\panel");
	SetMeshVisibilityMode(AddMesh(panelMesh, &paneloffset), MESHVIS_VC);

	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 10; y++) {
			int id = x + y * 5 + 1;
			VECTOR3 panelPosition = _V(0.8 - (x * 0.4), 0.05, 0.9 - y * 0.2);
			testSwitches.push_back(new PrototypeSwitch(id, panelPosition));
		}
	}
//	testSwitches.push_back(new PrototypeSwitch(1, _V(-0.8, 0.05, 0.9)));
//	testSwitches.push_back(new PrototypeSwitch(2, _V(-0.4, 0.05, 0.9)));
//	testSwitches.push_back(new PrototypeSwitch(3, _V(-0.4, 0.05, 0.7)));

	for (const auto& it : testSwitches) {
		it->init(this, eventBroker);
	}

	// Event will be propagated in first clbkPreStep
	eventBroker.publish(EVENTTOPIC::GENERAL, new SimpleEvent(EVENTTYPE::SIMULATIONSTARTEDEVENT));

}


void OrbitalHauler::clbkPreStep(double  simt, double  simdt, double  mjd) {
	
	// Propagate due events.
	// This should always remain at the beginning of clbkPreStep and never be called anywhere else.
	eventBroker.processEvents();

}

bool OrbitalHauler::clbkLoadVC(int id) {

	SetCameraOffset(_V(0, 1, -0.5));
	for (const auto& it : testSwitches) {
		it->loadVC();
	}

	return true;
}

bool OrbitalHauler::clbkVCMouseEvent(int id, int event, VECTOR3& p) {

	eventBroker.publish(EVENTTOPIC::UI_VC, new MouseEvent(id, event));
	return true;
}

bool OrbitalHauler::clbkVCRedrawEvent(int  id, int  event, SURFHANDLE  surf) {

	bool bugme = true;
	return false;
}


