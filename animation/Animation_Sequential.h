#pragma once

/**
 * \brief A sequential animation has a clear begining and end and can move in both directions between them. 
 */
class Animation_Sequential :
	public Animation_Base
{
public:
	/**
	 * \param _data Pointer to the data defining this animation
	 */
	Animation_Sequential(ANIMATIONDATA *_data);
	~Animation_Sequential();

	virtual AnimationEvent_Base *PropagateAnimation(double simdt);


};

