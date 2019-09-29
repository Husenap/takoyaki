#pragma once

#include "AnimationTrack.h"

namespace ty {

class AnimationSystem {
public:
	AnimationSystem();
	~AnimationSystem();

	std::vector<AnimationTrack>& GetAnimationTracks() { return mAnimationTracks; }
	int GetTrackCount() { return (int)mAnimationTracks.size(); }

private:
	std::vector<AnimationTrack> mAnimationTracks;
};

}  // namespace ty