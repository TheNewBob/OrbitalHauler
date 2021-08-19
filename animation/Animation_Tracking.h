#pragma once


/**
 * \brief An animation that can track a specified target in two axes.
 *
 * For both animations, a radial range can be defined. The animation will notice itself when the target is outside
 * its allowed radial range and inform of that fact with an appropriate TrackingAnimationStatusEvent.
 *
 * A tracking animation does in fact consist of two independant rotation animations, a primary (aligned first) and a secondary. 
 * Tracking is achieved by calculating a directional vector to the target, moving one animation until the apropriate axes 
 * are aligned, modifying the second animation's axis to align with the first animation, and then runs the second animation
 * until all axes are aligned.
 * Compared to the other animations, it's a rather complex beast.
 */
class Animation_Tracking :
	public Animation_Base
{
public:
	/**
	* \param _data Pointer to the data defining this animation
	*/
	Animation_Tracking(ANIMATIONDATA *_data);
	~Animation_Tracking();

	virtual void AddAnimationToVessel(VESSEL4 *_vessel, int _meshindex, MATRIX3 moduleorientation, VECTOR3 modulelocalpos);
	virtual AnimationEvent_Base *PropagateAnimation(double simdt);
	virtual AnimationEvent_Base *StartAnimation(StartAnimationEvent *startevent);
	virtual void ModifyAnimation(ModifyAnimationEvent *modifyevent);
	virtual void RemoveAnimationFromVessel();
	virtual void StopAnimation();
	virtual string GetScenarioLine();
	virtual void InitStateFromScenario(vector<string> &line);

protected:
	OBJHANDLE target = NULL;				//!< Handle to the object in orbiter the animation is supposed to track
	VECTOR3 facing;							//!< The directional vector of the origin state of the animation (vessel relative)
	VECTOR3 primaryaxis;					//!< The primary axis of rotation transformed to vessel internal frame
	VECTOR3 secondaryaxis;					//!< the secondary axis of rotation transformed to vessel internal frame
	int secondaryorbiterid = -1;			//!< identifier that identifies the secondary animation in orbiter
	double secondarystate = 0.0;			//!< The state of the secondary animation
	double secondaryspeed = 0.0;			//!< The speed of the secondary animation
	double primarytargetstate = 0.0;		//!< the state the primary animation is aiming to achieve
	double secondarytargetstate = 0.0;		//!< the state the secondary animation is aiming to achieve
	double lastalignement = 0.0;			//!< time since the last alignement (check of target position) in miliseconds
	MGROUP_ROTATE *secondrotation;			//!< Pointer to the component of the secondary animation
	bool stopanimation = false;				//!< Sets to true when the animation needs to stop and return to origin
	bool initmovinganim = false;			//!< True if the animation is already moving when initialising.

	virtual void EnableAnimation();			
	virtual void DisableAnimation();
	
	/**
	 * \brief sets the facing to origin
	 *
	 * needs to be called whenever animation is added to new vessel!
	 */
	virtual void initFacing();

	/**
	 * \brief Calculates the animation states at which the facig vector will be pointing towards the target
	 */
	void calculateTargetState();
	
	/**
	 * \brief Determines the direction the rotation has to move in
	 *
	 * \param targetstate The state the animation has to aim for
	 * \param currentstate The state the animation is in currently
	 * \param canloop pass true if the animation can loop, i.e. has a 360 degree range
	 * \return 1 or -1, indicating whether the rotation should move forward or backward
	 */
	int determineMovementDirection(double targetstate, double currentstate, bool canloop);
	
	/**
	 * \brief propagates the passed state toward a target state
	 * \param _state The current state of the animation (will be modified by function!!)
	 * \param _targetstate The state the animation has to reach
	 * \param _speed The speed and direction of the animation
	 * \param simdt Elapsed time since last call in miliseconds
	 * \param canloop pass true if the animation can loop
	 * \return True if the targeted state was reached, false otherwise
	 */
	bool propagateState(double &_state, double _targetstate, double _speed, double simdt, bool canloop);

	/**
	 * \brief Initialises the secondary rotation axis of the animation on creation
	 * \note only needs to be called if the tracking animation is in a state of movement when being created
	 */
	void initMovingAnimation();
};
