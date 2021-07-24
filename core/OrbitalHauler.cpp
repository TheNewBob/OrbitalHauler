
#define STRICT
#define ORBITER_MODULE

#include "core\Common.h"
#include "core\OrbitalHauler.h"



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

OrbitalHauler::~OrbitalHauler() { }

void OrbitalHauler::clbkSetClassCaps(FILEHANDLE cfg) {

	Olog::setLogLevelFromFile(cfg);
	Olog::info("Log level set to %i", Olog::loglevel);

}



