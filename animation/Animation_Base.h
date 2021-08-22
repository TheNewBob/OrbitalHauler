#pragma once

/**
 * Abstract base class for animations
 */
class Animation_Base
{
public:
	/**
	 * \param _data Pointer to the data that describes the animation
	 */
	Animation_Base(ANIMATIONDATA *_data);
	virtual ~Animation_Base();

	/**
	 * \brief Adds the animation to the vessel using the orbiter API
	 * \param _vessel The vessel to add the animation to
	 * \param _meshindex The index of the mesh that is influenced by this animation
	 * \param moduleorientation Matrix to transform from module-relative to vessel-relative coordinates
	 * \param modulelocalpos Vessel-relative position of the module containing this animation
	 */
	virtual void AddAnimationToVessel(VESSEL4 *_vessel);

	/**
	 * \brief Removes the animation from the vessel
	 */
	virtual void RemoveAnimationFromVessel();
	

	/**
	 * \brief Initialises the animation state from a scenario line
	 * \param line A list of strings, representing the scenario line cut by " "
	*/
	virtual void InitStateFromScenario(vector<string> &line);


	/**
	 * \brief propagates the animation and returns an AnimationFinishedEvent if the animation stoped.
	 * 
	 * propagation behavior has to be implemented by child classes.
	 * \param simdt The time elapsed since the last frame
	 */
	virtual AnimationEvent_Base *PropagateAnimation(double simdt)=0;
	
	/**
	 * \brief Starts the animation
	 * \param startevent The StartAnimationEvent that triggers the start of the animation
	 */
	virtual AnimationEvent_Base *StartAnimation(StartAnimationEvent *startevent);

	/**
	 * \brief Modifies an animation, usually its speed and direction
	 * \param modifyevent The event that modifies the animation
	 */
	virtual void ModifyAnimation(ModifyAnimationEvent *modifyevent);
	
	/**
	 * \brief Stops the animation.
	 * \note stopping behavior is implemented by inheriting classes
	 */
	virtual void StopAnimation();
	
	/**
	 * \return True if the animation is currently running, false otherwise.
	 * \note direction is not considered
	 */
	bool IsRunning();

	/**
	 * \return a string describing the state of the animation to write into a scenario file. 
	 */
	virtual string GetScenarioLine();

	/**
	 * \brief Disables updating of the animation state in orbiter.
	 *
	 * Animations should be disabled when a vessel visual does not exist (not close enough to camera) or if the whole vessel doesn't exist.
	 * The animation state will still propagate even if disabled, so when the vessel are created it will be as if the animation never stoped
	 */
	virtual void DisableAnimation();
	
	/**
	 * \brief Enables updating of the animation state in orbiter
	 * \see DisableAnimation()
	 */
	virtual void EnableAnimation();

	/**
	 * \return The current state of the animation 
	 */
	double GetState() { return state; };

	/**
	 * \brief Fills the passed reference with the dependencies of this animation 
	*/
	void GetDependencies(vector<ANIMATIONDEPENDENCY> &OUT_dependencies) { OUT_dependencies = data->dependencies; };

protected:
	VESSEL4 *vessel;													//!< The vessel this animation is currently a part of
	ANIMATIONDATA *data;											//!< Pointer to the data defining this animation
	double speed = 0.0;												//!< The current speed/direction of the animation
	int orbiterid;													//!< The orbiter-generated identifier for this animation
	double state = 0.0;												//!< The current state of the animation (>= 0.0 <= 1.0)
	double maxspeed = 1e12;											//!< The maximum speed the animation can have. Effects not yet implemented
	vector<UINT*> animationgroups;									//!< List meshgroup indices this animation acts on
	vector<ANIMATIONCOMPONENT_HANDLE> animationcomponents;			//!< List of orbiter-internal components this animation consists of

	bool enabled = false;											//!< An animation is disabled when the vessel visual doesn't exist. state still gets propagated, but the animation state on the vessel doesn't get updated	
	
	/**
	 * \brief Creates a rotation animation component that can be added to the animation on the vessel
	 * \param comp The data to create the component from
	 * \param meshRotation Transformation matrix from vessel-relative to mesh-relative orientation
	 * \return A pointer to the created MGROUP_ROTATE object
	 * \note Once the component is added to an orbiter animation, it must be destroyed by destroying the animation with oapi calls.
	 *	Destroying the object otherwise will lead to a crash. Not adding the returned component to an orbiter animation will result in a 
	 *	memory leak.
	 * \see createTranslationComponent()
	 * \see createScaleComponent()
	 */
	MGROUP_ROTATE *createRotationComponent(ANIMCOMPONENTDATA *comp, MATRIX3 meshRotation);
	
	/**
 	 * \brief Creates a translation animation component that can be added to the animation on the vessel
	 * \param comp The data to create the component from
	 * \param meshRotation Transformation matrix from vessel-relative to mesh-relative orientation
	 * \return A pointer to the created MGROUP_TRANSLATE object
	 * \note Once the component is added to an orbiter animation, it must be destroyed by destroying the animation with oapi calls.
	 *	Destroying the object otherwise will lead to a crash. Not adding the returned component to an orbiter animation will result in a
	 *	memory leak.
	 * \see createRotationComponent()
	 * \see createScaleComponent()
	 */
	MGROUP_TRANSLATE *createTranslationComponent(ANIMCOMPONENTDATA *comp, MATRIX3 meshRotation);
	
	/**
	 * \brief Creates a scale animation component that can be added to the animation on the vessel
	 * \param comp The data to create the component from
	 * \param meshRotation Transformation matrix from vessel-relative to mesh-relative orientation
	 * \return A pointer to the created MGROUP_ROTATE object
	 * \note Once the component is added to an orbiter animation, it must be destroyed by destroying the animation with oapi calls.
	 *	Destroying the object otherwise will lead to a crash. Not adding the returned component to an orbiter animation will result in a
	 *	memory leak.
	 * \see createTranslationComponent()
	 * \see createRotationComponent()
	 * \todo As there have been no scaling animations yet, this is untested!
	 */
	MGROUP_SCALE *createScaleComponent(ANIMCOMPONENTDATA *comp, MATRIX3 meshRotation);

	UINT createAnim(double initial_state);
};

