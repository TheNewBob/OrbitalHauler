#pragma once

class ReactionControlSystem :
    public VesselSystem
{
public:
    ReactionControlSystem(ThrusterConfig config, OrbitalHauler* vessel);
    ~ReactionControlSystem();

    void init();

private:
    ThrusterConfig config;

};

