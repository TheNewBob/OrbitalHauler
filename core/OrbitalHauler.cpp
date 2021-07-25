
#define STRICT
#define ORBITER_MODULE

#include "core/Common.h"
#include "systems/VesselSystem.h"
#include "systems/mainengine/MainEngine.h"
#include "systems/rcs/ReactionControlSystem.h"
#include "systems/dockport/DockPort.h"

#include "core/OrbitalHauler.h"



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

OrbitalHauler::OrbitalHauler(OBJHANDLE hVessel, int flightmodel) : VESSEL4(hVessel, flightmodel) { }

OrbitalHauler::~OrbitalHauler() {
	
	// Delete vessel systems
	for (vector<VesselSystem*>::iterator it = systems.begin(); it != systems.end(); ++it) {
		delete (*it);
	}

}

void OrbitalHauler::clbkSetClassCaps(FILEHANDLE cfg) {

	Olog::setLogLevelFromFile(cfg);
	Olog::info("Log level set to %i", Olog::loglevel);

	// Initialise vessel systems
	systems.push_back(new MainEngine(this));
	systems.push_back(new ReactionControlSystem(this));
	systems.push_back(new DockPort(this));

	for (vector<VesselSystem*>::iterator it = systems.begin(); it != systems.end(); ++it) {
		(*it)->init();
	}

}



