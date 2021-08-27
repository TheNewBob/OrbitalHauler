#pragma once
class InstrumentPanel
{
public:
	
	/**
	 * \param vessel The vessel this panel is created for.
	 * \param width The width of the panel in meters.
	 * \param height The height of the panel in meters.
	 * \param position position of the panels center relative to the vessels origin.
	 * \param facingDirection Direction the panel is facing (i.e. the "front" of the panel), as a normalized vector.
	 * \param upDirection which way the panels top edge points, as a normalized vector.
	 * \note For example, facingDirection (0, 0, -1) and upDirection (0, 1, 0) is a panel that is mounted flat to the "front wall", right side up, facing towards the rear of the vessel.
	 */
	InstrumentPanel(VESSEL4* vessel, double width, double height, VECTOR3 position, VECTOR3 facingDirection, VECTOR3 upDirection);
	~InstrumentPanel();

	void init(EventBroker *eventBroker, EVENTTOPIC receiverTopic, AnimationManager *animationManager);
	void loadVc();
	void addElement(InstrumentPanelElement* element, double x, double y);

	/**
	 * Pass data received from clbkVCMouseEvent to here.
	 */
	bool processMouseEvent(int areaId, int event, VECTOR3& position);
	
	void visualCreated(VISHANDLE vis);

private:
	MESHHANDLE mesh = NULL;
	UINT meshIndex = -1;
	MATRIX3 rotationMatrix;
	
	VECTOR3 position;
	VECTOR3 facingDirection;
	VECTOR3 upDirection;
	VECTOR3 scale;

	vector<InstrumentPanelElement*> elements;
	bool isInitialised = false;
	VESSEL4* vessel;
	bool drawBackground = true;

	VECTOR3 calculateElementsAbsolutePosition(InstrumentPanelElement *element, MATRIX3& panelRotation);
};

