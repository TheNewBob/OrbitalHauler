#pragma once

/**
 * Abstract base class for various elements on an instrument panel.
 */
class InstrumentPanelElement
{
public:
	InstrumentPanelElement(VESSEL4* vessel, EventBroker &eventBroker, EVENTTOPIC receiverTopic, int vcAreaId, VECTOR3 position);
	virtual ~InstrumentPanelElement();
	virtual void init(VECTOR3 &absoluteElementPosition, MATRIX3 &panelRotation) = 0;
	virtual void loadVc(VECTOR3& elementAbsolutePosition) = 0;
	virtual bool processMouseEvent(int event, VECTOR3& position) = 0;
	virtual void visualCreated(VISHANDLE vis, MATRIX3 &panelRotation) = 0;

	int getVcAreaId() { return vcAreaId; };
	VECTOR3& getPosition() { return position; };


protected:
	/**
	 * ID of the virtual cockpit area registered with orbiter that this element is mapped to. 
	 */ 
	int vcAreaId = -1;

	VECTOR3 position;
	VECTOR3 rotation;
	VECTOR3 scale;

	/**
	 * The vessel containing the cockpit containing the panel.
	 */
	VESSEL4* vessel = NULL;

	EventBroker& eventBroker;
	EVENTTOPIC receiverTopic;
};

