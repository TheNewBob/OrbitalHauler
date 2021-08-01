#pragma once

class VesselSystem : public EventSubscriber {

public:
	VesselSystem(OrbitalHauler* vessel) {
		this->vessel = vessel;
	};
	~VesselSystem() {};

	virtual void init() = 0;
	virtual void preStep(double simt, double simDt, double mjd) {};
	virtual void postStep(double simt, double simDt, double mjd) {};

protected:
	OrbitalHauler *vessel;

};

