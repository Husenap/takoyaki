#include "AnimationSystem.h"

namespace ty {

AnimationSystem::AnimationSystem() {
	mAnimationTracks.push_back(AnimationTrack("Scene"));
	mAnimationTracks.push_back(AnimationTrack("Camera1"));
	mAnimationTracks.push_back(AnimationTrack("Camera2"));
	mAnimationTracks.push_back(AnimationTrack("CA"));
	mAnimationTracks.push_back(AnimationTrack("DOF"));
	mAnimationTracks.push_back(AnimationTrack("Cool"));
	mAnimationTracks.push_back(AnimationTrack("MoreCool"));
	mAnimationTracks.push_back(AnimationTrack("EvenMoreCool"));
	mAnimationTracks.push_back(AnimationTrack("SuperCool"));
	mAnimationTracks.push_back(AnimationTrack("DreamBig"));
}

AnimationSystem::~AnimationSystem() {}

}  // namespace ty