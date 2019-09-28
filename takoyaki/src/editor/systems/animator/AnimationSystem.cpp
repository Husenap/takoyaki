#include "AnimationSystem.h"

namespace ty {

AnimationSystem::AnimationSystem() {
	mAnimationTracks.push_back(AnimationTrack("Scene", UniformType::Float));
	mAnimationTracks.push_back(AnimationTrack("Camera 1", UniformType::Vec3));
	mAnimationTracks.push_back(AnimationTrack("Camera 2", UniformType::Vec3));
	mAnimationTracks.push_back(AnimationTrack("Chromatic Aberration", UniformType::Float));
	mAnimationTracks.push_back(AnimationTrack("Depth of Field", UniformType::Float));
	mAnimationTracks.push_back(AnimationTrack("Something Cool", UniformType::Vec2));
	mAnimationTracks.push_back(AnimationTrack("Something More Cool", UniformType::Vec3));
	mAnimationTracks.push_back(AnimationTrack("Something Even Cool", UniformType::Vec4));
	mAnimationTracks.push_back(AnimationTrack("Something Super Cool", UniformType::Color));
	mAnimationTracks.push_back(AnimationTrack("Dream big", UniformType::Vec4));
}

AnimationSystem::~AnimationSystem() {}

}  // namespace ty