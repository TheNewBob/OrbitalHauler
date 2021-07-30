#include "EventTypes.h"
#include "Event_Base.h"
#include "Event_Timed.h"
#include <time.h>


Event_Timed::Event_Timed(EVENTTYPE _type, unsigned int miliseconds) : Event_Base(_type, miliseconds)
{
	creationtime = clock();
}


Event_Timed::~Event_Timed()
{
}



bool Event_Timed::sendMe()
{
	unsigned long int currenttime = clock();

	if (creationtime + (delay / 1000.0 * CLOCKS_PER_SEC)  <= currenttime)
	{
		return true;
	}
	return false;
}
