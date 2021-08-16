#pragma once

/**
 * Abstract base class for various elements on an instrument panel.
 */
class InstrumentPanelElement
{
public:
	InstrumentPanelElement(VESSEL4* vessel, int vcAreaId);
	virtual ~InstrumentPanelElement();

	/**
	 * Initialises the element in relation to its containing panel. Invoked from InstrumentPanel.init(), do not call yourself!
	 */
	void init(VECTOR3 &elementAbsolutePosition, EventBroker *eventBroker, EVENTTOPIC receiverTopic, MATRIX3 &panelRotation);
	
	/**
	 * Performs positioning and area registration when clbkLoadVc is called. Invoked from InstrumentPanel.loadVc(), do not call yourself!
	 */
	virtual void loadVc(VECTOR3& elementAbsolutePosition) = 0;

	/**
	 * Processes mouse events that have been received for the elements vcAreaId. Invoked from InstrumentPanel.processMouseEvent(), do not call yourself!
	 */
	virtual bool processMouseEvent(int event, VECTOR3& position) = 0;

	/**
	 * Performs required mesh transformations when the vessels visuals are created by the core. Invoked from InstrumentPanel.visualCreated(), do not call yourself!
	 */
	virtual void visualCreated(VISHANDLE vis, MATRIX3 &panelRotation);

	int getVcAreaId() { return vcAreaId; };
	/**
	 * \return The position of the element relative to the panels 0/0.
	 * \note Is always 0, 0, 0 before init().
	 */
	VECTOR3& getPanelRelativePosition() { return position; };
	
	/**
	 * Sets the position of the element relative to the panels lower-left corner. Should only be set by the containing panel.
	 */
	void setPanelRelativePosition(VECTOR3& position) { this->position = position; };


protected:
	/**
	 * ID of the virtual cockpit area registered with orbiter that this element is mapped to. 
	 */ 
	int vcAreaId = -1;

	/**
	 * Position of the element relative to the containing panels lower-right corner. Z is unused! 
	 */
	VECTOR3 position = _V(0, 0, 0);
	
	/**
	 * Scaling transformation for the element's mesh.
	 * TODO: Scaling not yet implemented.
	 */
	VECTOR3 scale = _V(1, 1, 1);

	/**
	 * The vessel containing the cockpit containing the panel.
	 */
	VESSEL4* vessel = NULL;

	EventBroker *eventBroker = NULL;
	EVENTTOPIC receiverTopic;
	MESHHANDLE mesh = NULL;
	UINT meshIndex = -1;


	/**
	 * Override this method to initialise MESHHANDLE this->mesh and add it to the vessel at elementAbsolutePosition.
	 * You are free to do other initialisations, most commonly animations, or whatever else you need to set up for your specific element implementation.
	 * But the mesh *must* be initialised, and *must* be part of the vessel and in the proper position when this method returns, or there will be mayhem.
	 * You do *not* need to rotate the mesh to the panel, this action will be performed by the base class.
	 * You *do* however need to rotate your animation references yourself if you have any, use the passed rotation matrix for that.
	 * this-meshIndex will already be initialised in when this method is called, so do not overwrite it. You are free to use it if you need it, though.
     * 
	 * The major reason why mesh loading is left to the implementation is so the implementation can freely choose the source of the mesh, which must not necessarily be loaded from file.
	 */
	virtual void initElement(MATRIX3& panelRotation, VECTOR3 &elementAbsolutePosition) = 0;
};

