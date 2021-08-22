#pragma once
class AnimationManager
{
public:
	AnimationManager(VESSEL4* vessel);
	~AnimationManager();
	void addAnimation(ANIMATIONDATA *data);
	void removeAnimation(ANIMATIONDATA* data);
	void preStep(double simdt);

	/**
	 * Initialises the manager and adds all previously added animations animations to the vessel. 
	 */
	void init();

private:
	VESSEL4* vessel;
	bool isInitialised = false;
	
	// TODO: String ids are really not very efficient, but let's get this to work before optimising it.
	map<string, Animation_Base*> animations;
};

