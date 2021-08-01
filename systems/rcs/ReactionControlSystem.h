#pragma once

class ReactionControlSystem :
    public VesselSystem
{
public:
    ReactionControlSystem(ThrusterConfig config, OrbitalHauler* vessel);
    ~ReactionControlSystem();

    void init();

protected:
    virtual void receiveEvent(Event_Base* event);

private:
    ThrusterConfig config;

};

