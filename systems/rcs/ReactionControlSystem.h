#pragma once

class ReactionControlSystem :
    public VesselSystem
{
public:
    ReactionControlSystem(OrbitalHauler* vessel);
    ~ReactionControlSystem();

    void init();

};

