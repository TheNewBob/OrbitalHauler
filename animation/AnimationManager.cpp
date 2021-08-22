#include "core/Common.h"
#include "OpForwardDeclare.h"
#include "event/Events.h"

#include "AnimationData.h"
#include "Animation_Base.h"
#include "Animation_Sequential.h"
#include "Animation_Continuous.h"
#include "Animation_Tracking.h"

#include "AnimationManager.h"


AnimationManager::AnimationManager(VESSEL4 *vessel) : vessel(vessel) {}

AnimationManager::~AnimationManager() {
	// TODO: delete animations!
}

void AnimationManager::addAnimation(ANIMATIONDATA *data) {
	
	Animation_Base* newanim = NULL;
	//create appropriate animation object
	if (data->type == "sequence")
	{
		newanim = new Animation_Sequential(data);
	}
	else if (data->type == "continuous")
	{
		newanim = new Animation_Continuous(data);
	}
	else if (data->type.substr(0, 5) == "track")
	{
		newanim = new Animation_Tracking(data);
	}
	animations.insert(make_pair(data->id, newanim));

	if (isInitialised) {
		newanim->AddAnimationToVessel(vessel);
	}
}

void AnimationManager::removeAnimation(ANIMATIONDATA* data) {
	auto it = animations.find(data->id);
	Olog::assertThat([&]() { return it != animations.end(); }, "Removing an animation that was never added!");
	
	if (isInitialised) {
		it->second->RemoveAnimationFromVessel();
	}
	animations.erase(it);
}

void AnimationManager::init() {
	Olog::assertThat([&]() { return !isInitialised; }, "AnimationManager.init() is being called twice!");

	for (const auto& it : animations) {
		it.second->AddAnimationToVessel(vessel);
	}

}

void AnimationManager::preStep(double simdt) {

	//iterate through animation states
	for (auto i = animations.begin(); i != animations.end(); ++i)
	{
		//tell all animations to update
		auto animationEvent = (i->second->PropagateAnimation(simdt));

		//TODO: Publish events!
	}
}

