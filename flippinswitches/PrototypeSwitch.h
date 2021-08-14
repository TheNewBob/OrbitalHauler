#pragma once

/**
* Currently a placeholder for some prototyping for VC switches. Intended to be refactored into something sensible later.
*/
class PrototypeSwitch : public InstrumentPanelElement
{
public:
	PrototypeSwitch(VESSEL4* vessel, EventBroker& eventBroker, EVENTTOPIC receiverTopic, int vcAreaId, VECTOR3 position);
	~PrototypeSwitch();
	void init(VECTOR3& panelPosition, MATRIX3& panelRotation);
	void loadVc();
	void visualCreated(VISHANDLE vis, MATRIX3& panelRotation);
	
	bool processMouseEvent(int event, VECTOR3& position);


private:
	void toggle();

	MESHHANDLE mesh = NULL;
	UINT animationId = -1;
	UINT meshIndex = 0;
	bool state = false;
	MGROUP_ROTATE *animComponent = NULL;
	UINT* meshGroups = NULL;
};

