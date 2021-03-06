#pragma once


class MainEngine :
    public VesselSystem
{
public:
	MainEngine(ThrusterConfig config, OrbitalHauler *vessel);
	~MainEngine();

	void init(EventBroker& eventBroker);

protected:
	virtual void receiveEvent(Event_Base* event, EVENTTOPIC topic);

private:
	ThrusterConfig config;
};

