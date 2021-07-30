
/**
 * \brief Only fired in the first pre-step of a vessels existance. 
 *
 * Anybody having to do initialisations at sim-start that cannot be done previously
 * should do so in reaction to this event.
 */
class SimulationStartedEvent :
	public Event_Base
{
public:
	SimulationStartedEvent() : Event_Base(SIMULATIONSTARTEDEVENT, 1){};
	~SimulationStartedEvent(){};
};