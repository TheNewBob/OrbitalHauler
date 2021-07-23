
#define STRICT
#define ORBITER_MODULE

#include "orbitersdk.h"
#include "OrbitalHauler.h"

// Module Init and cleanup
DLLCLBK VESSEL* ovcInit(OBJHANDLE hvessel, int flightmodel)
{
	return new OrbitalHauler(hvessel, flightmodel);
}

DLLCLBK void ovcExit(VESSEL* vessel)
{
	if (vessel) delete (OrbitalHauler*)vessel;
}


// Vessel class

OrbitalHauler::OrbitalHauler(OBJHANDLE hVessel, int flightmodel) : VESSEL3(hVessel, flightmodel) { }

OrbitalHauler::~OrbitalHauler() { }

void OrbitalHauler::clbkSetClassCaps(FILEHANDLE cfg) {

	bool bugme = true;

}



