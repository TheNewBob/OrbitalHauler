#pragma once

/**
* Currently a placeholder for some prototyping for VC switches. Intended to be refactored into something sensible later.
*/
class PrototypeSwitch
{
public:
	PrototypeSwitch(int panelId, VECTOR3 position);
	~PrototypeSwitch();
	void init(VESSEL4* vessel);
	void loadVC();
	void triggerSwitch();

private:
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

