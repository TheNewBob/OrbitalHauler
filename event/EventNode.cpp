#include "core/Common.h"
#include "EventTypes.h"
#include "Event_Base.h"
#include "EventNode.h"
#include "EventSink.h"


EventNode::EventNode()
{
}


EventNode::~EventNode()
{
	//event nodes are deallocated by their hosting EventHandlers
}


//registers another event sink as a child
void EventNode::RegisterEventSink(EventSink *sink)
{
	children.push_back(sink);
}




/* unregisters an event sink. returns true if succesful, false if the passed node was not a child
 * Does NOT delete the child that is being unregistered!
 */
bool EventNode::UnregisterEventSink(EventSink *sink)
{
	//check if the node is a member
	vector<EventSink*>::iterator i = find(children.begin(), children.end(), sink);
	if (i != children.end())
	{
		//found the node
		children.erase(i);
		children.shrink_to_fit();
	}
	else
	{
		//node was not found
		return false;
	}
	return true;
}



/* sends an Event to all children, breaks if one of the children consumes the event, and returns whether the event was consumed. 
 * must be overloaded and called from the child class
 */
bool EventNode::processEvent(Event_Base *_event)
{
	for (UINT i = 0; i < children.size(); ++i)
	{
		//if the event was consumed, don't continue propagating
		if (children[i]->processEvent(_event)) return true;
	}
	return false;
}
