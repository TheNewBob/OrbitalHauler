#include "core/Common.h"
#include "event/Events.h"
#include "InstrumentPanelElement.h"


InstrumentPanelElement::InstrumentPanelElement(VESSEL4* vessel, EventBroker& eventBroker, EVENTTOPIC receiverTopic, int vcAreaId, VECTOR3 position):
	vessel(vessel), eventBroker(eventBroker), receiverTopic(receiverTopic), vcAreaId(vcAreaId), position(position) {}

InstrumentPanelElement::~InstrumentPanelElement() {}







