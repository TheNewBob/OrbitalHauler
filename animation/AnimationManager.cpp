#include "core/Common.h"
#include "OpForwardDeclare.h"
#include "event/Events.h"

#include "AnimationData.h"
#include "Animation_Base.h"
#include "Animation_Sequential.h"
#include "Animation_Continuous.h"
#include "Animation_Tracking.h"

#include "AnimationManager.h"


AnimationManager::AnimationManager(VESSEL4 *vessel, EventBroker &eventBroker) : vessel(vessel), eventBroker(eventBroker), EventSubscriber() {
	
}

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
	else {
		Olog::error("Invalid animation type: %s; Must be any of 'sequence', 'continuous', or 'track' (case sensitive!)");
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

	eventBroker.subscribe(this, EVENTTOPIC::ANIMATION);

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

		eventBroker.publish(EVENTTOPIC::ANIMATION, animationEvent);
	}
}

void AnimationManager::receiveEvent(Event_Base* event, EVENTTOPIC topic) {

	AnimationEvent_Base* animationEvent = dynamic_cast<AnimationEvent_Base*>(event);

	if (animationEvent != NULL) {
		auto animation = animations.find(animationEvent->GetAnimationId());
		if (animation == animations.end()) {
			Olog::warn("call to nonexisting animation: %s", animationEvent->GetAnimationId().data());
		} else {

			if (*event == EVENTTYPE::STARTANIMATIONEVENT) {
				//check if the start of the animation is hindered by dependencies
				StartAnimationEvent* startEvent = (StartAnimationEvent*)animationEvent;
				if (canStart(animation->second, startEvent->GetSpeed()))
				{
					eventBroker.publish(EVENTTOPIC::ANIMATION, animation->second->StartAnimation(startEvent));
				}
				else
				{
					eventBroker.publish(EVENTTOPIC::ANIMATION, new AnimationFailedEvent(startEvent->GetAnimationId(), startEvent->GetSpeed()));
				}
			}

			else if (*event == EVENTTYPE::MODIFYANIMATIONEVENT) {
				animation->second->ModifyAnimation((ModifyAnimationEvent*)animationEvent);
			}

			else if (*event == EVENTTYPE::STOPANIMATIONEVENT)
			{
				//stop the animation in question
				animation->second->StopAnimation();
			}
		}
	}
}

bool AnimationManager::canStart(Animation_Base* anim, double speed) {
	vector<ANIMATIONDEPENDENCY> deps;
	anim->GetDependencies(deps);

	for (UINT i = 0; i < deps.size(); ++i)
	{
		//find the dependency animation
		auto dep = animations.find(deps[i].dependencyid);
		if (dep == animations.end())
		{
			Olog::warn("dependency to nonexisting animation: %s", deps[i].dependencyid.data());
		}
		else
		{
			//check if the dependency applies to the direction the animation should move in
			if (speed > 0 && deps[i].direction > 0 ||
				speed < 0 && deps[i].direction < 0)
			{
				//check if the dependency is in a state where it prevents the animation from starting
				if (dep->second->GetState() != deps[i].dependencystate)
				{
					//if only one dependency doesn't check out, the animation cannot start
					return false;
				}
			}
		}
	}
	return true;
}


