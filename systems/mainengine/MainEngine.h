#pragma once


class MainEngine :
    public VesselSystem
{
public:
	MainEngine(ThrusterConfig config, OrbitalHauler *vessel);
	~MainEngine();

	void init();

private:
	ThrusterConfig config;
};

