#pragma once
class InstrumentPanel
{
public:
	InstrumentPanel(VESSEL4* vessel);
	~InstrumentPanel();

	void init();
	void loadVc();
	void addElement(InstrumentPanelElement* element);

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
	VECTOR3 rotation;
	VECTOR3 direction;
	VECTOR3 scale;

	vector<InstrumentPanelElement*> elements;
	bool isInitialised = false;
	VESSEL4* vessel;
	bool drawBackground = true;
};

