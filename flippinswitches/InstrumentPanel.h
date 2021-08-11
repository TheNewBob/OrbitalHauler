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

private:
	MESHHANDLE mesh = NULL;
	
	VECTOR3 position;
	VECTOR3 rotation;
	VECTOR3 scale;

	vector<InstrumentPanelElement*> elements;
	bool isInitialised = false;
	VESSEL4* vessel;
	bool drawBackground = true;
};

