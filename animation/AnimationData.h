#pragma once

struct ANIMCOMPONENTDATA
{
	vector<int> groups;
	string type = "";
	VECTOR3 reference;
	VECTOR3 axis;
	float range;
	int parent = -1;
	vector<double> duration = { 0,1 };
	Oparse::OpModelDef GetModelDef();
};

/**
 * \brief defines a dependency for an animation.
 *
 * a dependency is a set state of another animation
 * that must be fulfilled in order for this animation
 * to be able to execute
 */
struct ANIMATIONDEPENDENCY
{
	int direction;					//for which direction of THIS animation the dependency applies (<-1 reverse, 1 normal)
	string dependencyid;			//the identifier of the animation this animation depends on
	double dependencystate;			//the state of the other animation this animation depends on
	Oparse::OpMixedList* GetMapping();
};

/**
 * Used to store static data of a specific animation
 */
struct ANIMATIONDATA
{
	string id = "";
	double duration;
	string type = "";
	VECTOR3 facing;
	vector<ANIMCOMPONENTDATA*> components;
	vector<ANIMATIONDEPENDENCY> dependencies;

	Oparse::OpModelDef GetModelDef();
};

/**
 * \brief Defines an arbitrary amount of animations as a module function.
 *
 * Note that all parts of this animation must be contained in and limited to the specific module it is declared in!
 *
 * <h3> Config file parameters </h3>
 * IMS2 handles animations as part of a module function just like any other module function. Contrary to other module functions, though,
 * an animation module function has no purpose unless the animations inside it are referenced by other module functions in the same module.
 * As a result, an animation module function usually consists of several animations, which again consist of several animation components.
 * These components generally use the same data that is used by orbiter itself and provides a syntax familiar to SC3 ini files.
 *
 * \code
 * BEGIN_MODULEFUNCTION ANIMATION
 *	BEGIN_ANIM <string id>							//Begins an animation with the passed id. The id may NOT contain spaces and MUST be unique in the entire module.
 *		Type = <Sequence|Continuous|Track <x y z>>	//The type of the animation: Sequence means a sequential animation that will start, and then stop once it is finished.
 *													//Continuous is a looped animation that is intended to repeat after it has run.
 *													//Track is a special kind of animation used to align with an ingame object.
 *													//It needs a normalised vector that describes the direction it is pointing towards in its default state as an argument.
 *		Duration = <float seconds>					//The duration the whole animation will last (in case of continuous animations, the duration of one loop).
 *		[Dependency] = <see below>					//Lets you declare a dependency on another animation. See below for details.
 *		BEGIN_COMP <int index>						//Begins a block declaring an animation component. See below for details.
 *		END_COMP									//Ends an animation component block
 *	END_ANIM										//ends an animation block
 * END_MODULEFUNCTION
 * \endcode
 *
 * <h4> animation dependencies </h4>
 * Some animations may depend on another animation in this module. For example, a module that has two folded up antennas might have them folded
 * in a way that one antena is folded on top of the other, ergo the other can't deploy unless the first one is deployed, while the first one
 * cannot fold up unless the other one is folded up. An arbitrary number of dependencies can be defined for any animation.
 * \code
 * Dependency = <int direction> <string id> <float state>
 *	direction:	<-1 | 1>	//For which direction of THIS animation the dependency applies.
 *							//To get back to the initial example, the antena below cannot deploy unless the antena above it is deployed,
 *							//so the direction for which it is dependant on the other one is its usual direction, i.e. 1.
 *							//On the other hand, the upper antenna cannot be retracted unless the antena below it has been retracted,
 *							//so it has a dependency on that animation for the reverse of the deployment animation, i.e. -1.
 *	id: <string>			//The id of the animation this animation depends on.
 *  state: <float>			//The state the OTHER animation must be in for THIS animation to be able to execute.
 *							//Again refering to the above example, the lower antena cannot deploy until the upper antenna has deployed,
 *							//so the state would be 1.0 (upper antena has completely deployed). The uper antena, on the other hand,
 *							//would depend of the lower antena having a state of 0.0 (completely retracted) in order to retract.
 * \endcode
 *
 * <h4> animation components </h4>
 * Animation components work practically the same to how they work in orbiter or in SC3.
 * As such, it is highly recommended to read up on animations in the orbiter API guide (not the code, but the concepts).
 * \code
 * BEGIN_COMP ;[<int idx>]						//Note that the index is A COMMENT! It's not actually required, but noting it makes the file a LOT more readable. Trust me on this, you won't regret it!
 *	Groups = <array<int> indices>				//The mesh groups involved in this part of the animation. an arbitrary number of space-delimited mesh-group indices can be defined.
 *	Type = <Rotate|Translate|Scale>				//Whether this part of the animation rotates, translates or scales the referenced mesh groups.
 *	[Origin] = <float x> <float y> <float z>	//Required for rotation and scale, but not for translation. For rotations, this defines the center of rotation. For scale, it defines the origin from which to scale.
 *	[Axis] = <float x> <float y> <float z>		//Required for rotations. The axis around which to rotate, as a normalised vector.
 *  [Range] = <float>							//Required for rotations. The angle by how much the referenced meshgroups should be rotated, in degrees.
 *	[Translate] = <float x> <float y> <float z> //Required for translation. The magnitude and direction of the translation to be performed.
 *  [Scale] = <float x> <float y> <float z>		//Required for scaling. The magnitude of the scaling in the 3 principle axes.
 *	[Duration] = <float> <float>				//Where in the overall animation this component begins and where it ends. The values can range from 0 to 1,
 *												//and the first value must always be smaller than the second. Default is 0.0 to 1.0, i.e. duration of entire animation.
 *	[Parent] = <int idx>						//Makes this component the child of another component, i.e. transformations from the parent component will be applied
 *												//to this component before this component applies its own transformation.
 *												//Note that child components must ALWAYS be defined after their parent!
 * END_COMP
 * \endcode
 */
class AnimationData 
{

public:
	AnimationData();
	~AnimationData();

	Oparse::OpModelDef GetModelDef();

private:

	vector<ANIMATIONDATA*> animations;
};


