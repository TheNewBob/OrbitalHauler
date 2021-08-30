#pragma once

/**
* Currently a placeholder for some prototyping for VC switches. Intended to be refactored into something sensible later.
*/
class PrototypeSwitch : public InstrumentPanelElement, EventSubscriber
{
public:
	PrototypeSwitch(VESSEL4* vessel, int vcAreaId);
	~PrototypeSwitch();
	void initElement(MATRIX3 &panelRotation, VECTOR3& elementAbsolutePosition, AnimationManager* animationManager);
	void loadVc(VECTOR3& elementAbsolutePosition);
	
	bool processMouseEvent(int event, VECTOR3& position);

	void receiveEvent(Event_Base* event, EVENTTOPIC topic);


private:
	void toggleSwitch();

//	UINT animationId = -1;
	bool state = false;
	string animationId = "";

	ANIMATIONDATA* animationData = NULL;

};

