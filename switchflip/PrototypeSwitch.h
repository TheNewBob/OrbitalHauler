#pragma once

/**
* Currently a placeholder for some prototyping for VC switches. Intended to be refactored into something sensible later.
*/
class PrototypeSwitch : EventSubscriber
{
public:
	PrototypeSwitch(int panelId, VECTOR3 position);
	~PrototypeSwitch();
	void init(VESSEL4* vessel, EventBroker &eventBroker);
	void loadVC();
	
	void receiveEvent(Event_Base* event, EVENTTOPIC topic);


private:
	void toggle();

	int panelId = -1;
	MESHHANDLE mesh = NULL;
	VECTOR3 position;
	UINT animationId = -1;
	UINT meshIndex = 0;
	VESSEL4* vessel = NULL;
	bool state = false;
	MGROUP_ROTATE *animComponent = NULL;
	UINT* meshGroups = NULL;
};

