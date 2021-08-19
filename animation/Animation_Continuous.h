#pragma once

/**
 * \brief Implementation of a continuous animation
 *
 * A continuous animation will loop back to state 0.0 once itreaches state 1.0.
 * In case it gets a StopAnimationEvent, it will finish the current cycle to 1.0,
 * loop back to 0.0 and stop in its original position.
 */
class Animation_Continuous :
	public Animation_Base
{
public:
	/**
	 * \param _data Pointer to the data defining this animation
	 */
	Animation_Continuous(ANIMATIONDATA *_data);
	~Animation_Continuous();

	virtual AnimationEvent_Base *PropagateAnimation(double simdt);
	virtual void StopAnimation();

	virtual void InitStateFromScenario(vector<string> &line);
	virtual string GetScenarioLine();

private:
	bool stopmenexttime = false;				//!< Indicates whether the animation needs to stop when it reaches 0.0 the next time.

};

