#include "core/Common.h"
#include "EventTypes.h"
#include "Event_Base.h"
#include "EventHandler.h"
#include "EventNode.h"
#include "EventSink.h"


EventSink::EventSink(EventHandler *_host)
{
	host = _host;
}


EventSink::~EventSink()
{
}


/* first sends the event to the ProcessEvent function of its host, then forwards it to all its children
 * immediately breaks if the event was consumed down the line without further forwarding.
 * returns true if the event was consumed
 */
bool EventSink::processEvent(Event_Base* e)
{
	//an event has been passed. First, process it in this event handler
	bool consumed = host->ProcessEvent(e);

	//if the event hasn't been consumed, send it on to the children
	if (!consumed && EventNode::processEvent(e))
	{
		return true;
	}

	return consumed;
}
