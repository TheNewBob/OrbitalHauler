#pragma once

class Cockpit;

class OrbitalHauler : public VESSEL4 {
public:
	OrbitalHauler(OBJHANDLE hVessel, int flightmodel);
	~OrbitalHauler();
	void clbkSetClassCaps(FILEHANDLE cfg);
	void clbkPreStep(double  simt, double  simdt, double  mjd);
	bool clbkLoadVC(int id);

private:
	Cockpit *cockpit = 0;
	vector<VesselSystem*> systems;
	EventBroker eventBroker;
};