#pragma once


class DockPort :
    public VesselSystem
{
public:
    DockPort(OrbitalHauler* vessel);
    ~DockPort();

    void init();

protected:
    virtual void receiveEvent(Event_Base* event);
};

