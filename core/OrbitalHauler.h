#pragma once


class OrbitalHauler : public VESSEL4 {
public:
	OrbitalHauler(OBJHANDLE hVessel, int flightmodel);
	~OrbitalHauler();
	void clbkSetClassCaps(FILEHANDLE cfg);
	void clbkPreStep(double  simt, double  simdt, double  mjd);

private:
	vector<VesselSystem*> systems;
	EventBroker eventBroker;
};