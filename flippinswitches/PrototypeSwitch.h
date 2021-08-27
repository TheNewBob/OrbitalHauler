#pragma once

/**
* Currently a placeholder for some prototyping for VC switches. Intended to be refactored into something sensible later.
*/
class PrototypeSwitch : public InstrumentPanelElement
{
public:
	PrototypeSwitch(VESSEL4* vessel, int vcAreaId);
	~PrototypeSwitch();
	void initElement(MATRIX3 &panelRotation, VECTOR3& elementAbsolutePosition, AnimationManager* animationManager);
	void loadVc(VECTOR3& elementAbsolutePosition);
	
	bool processMouseEvent(int event, VECTOR3& position);


private:
	void toggle();

//	UINT animationId = -1;
	bool state = false;
	MGROUP_ROTATE *animComponent = NULL;
	UINT* meshGroups = NULL;
	string animationId = "";

	// TODO: Temporary for prototyping
	Animation_Sequential* animation = NULL;
	ANIMCOMPONENTDATA* rotation = NULL;
	ANIMATIONDATA* animationData = NULL;

};

