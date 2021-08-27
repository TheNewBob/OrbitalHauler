#pragma once
class AnimationManager : EventSubscriber
{
public:
	AnimationManager(VESSEL4* vessel, EventBroker &eventBroker);
	~AnimationManager();
	void addAnimation(ANIMATIONDATA *data);
	void removeAnimation(ANIMATIONDATA* data);
	void preStep(double simdt);
	void receiveEvent(Event_Base* event, EVENTTOPIC topic);

	/**
	 * Initialises the manager and adds all previously added animations animations to the vessel. 
	 */
	void init();

private:
	VESSEL4* vessel;
	bool isInitialised = false;
	EventBroker &eventBroker;
	
	// TODO: String ids are really not very efficient, but let's get this to work before optimising it.
	map<string, Animation_Base*> animations;

	/* \return false if the passed animation is prevented from starting
	 *	by one of its dependencies
	 */
	bool canStart(Animation_Base* anim, double speed);

};

