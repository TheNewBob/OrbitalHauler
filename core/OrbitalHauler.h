#pragma once


class OrbitalHauler : public VESSEL3 {
public:
	OrbitalHauler(OBJHANDLE hVessel, int flightmodel);
	~OrbitalHauler();
	void clbkSetClassCaps(FILEHANDLE cfg);


};