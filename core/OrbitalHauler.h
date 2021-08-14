#pragma once

class PrototypeSwitch;
class InstrumentPanel;

class OrbitalHauler : public VESSEL4 {
public:
	OrbitalHauler(OBJHANDLE hVessel, int flightmodel);
	~OrbitalHauler();
	void clbkSetClassCaps(FILEHANDLE cfg);
	void clbkPreStep(double  simt, double  simdt, double  mjd);
	bool clbkLoadVC(int id);
	bool clbkVCMouseEvent(int id, int event, VECTOR3& p);
	bool clbkVCRedrawEvent(int  id, int  event, SURFHANDLE  surf);
	void clbkVisualCreated(VISHANDLE vis, int refcount);

	VISHANDLE getVisHandle() { return hVis; };

private:
	vector<VesselSystem*> systems;
	EventBroker eventBroker;
	InstrumentPanel* panel = 0;
	
	/**
	 * Visual handle of the vessel.
	 */
	VISHANDLE hVis = NULL;


};