#pragma once


class MainEngine :
    public VesselSystem
{
public:
	MainEngine(OrbitalHauler *vessel);
	~MainEngine();

	void init();
};

