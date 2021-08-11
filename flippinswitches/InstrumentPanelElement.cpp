#include "core/Common.h"
#include "event/Events.h"
#include "InstrumentPanelElement.h"


InstrumentPanelElement::InstrumentPanelElement(VESSEL4* vessel, EventBroker& eventBroker, EVENTTOPIC receiverTopic): 
	vessel(vessel), eventBroker(eventBroker), receiverTopic(receiverTopic) {}

InstrumentPanelElement::~InstrumentPanelElement() {}







