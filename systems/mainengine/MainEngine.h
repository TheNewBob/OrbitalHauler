#pragma once


class MainEngine :
    public VesselSystem
{
public:
	MainEngine(ThrusterConfig config, OrbitalHauler *vessel);
	~MainEngine();

	void init();

protected:
	virtual void receiveEvent(Event_Base* event);

private:
	ThrusterConfig config;
};

