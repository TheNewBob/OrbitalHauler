#pragma once

class MouseEvent : public Event_Base {
public:
	MouseEvent(int vcId, int mEvent) : Event_Base(EVENTTYPE::MOUSEEVENT, 1), vcId(vcId), mEvent(mEvent) {};
	~MouseEvent() {};
	int getEvent() { return mEvent; };
	int getVcId() { return vcId; };

private:
	int mEvent;
	int vcId;

};