#pragma once

#include "AnimationTrack.h"
#include "../../ServiceManager.h"

namespace ty {

struct TrackValue {
	float mValue;
	std::string mName;
};

class AnimationSystem {
public:
	AnimationSystem(SyncSystem& syncSystem);
	~AnimationSystem();

	std::vector<AnimationTrack>& GetAnimationTracks() { return mAnimationTracks; }
	int GetTrackCount() { return (int)mAnimationTracks.size(); }

	std::vector<TrackValue> Evaluate(float time);

private:
	std::vector<AnimationTrack> mAnimationTracks;

	SyncSystem& mSyncSystem;
};

}  // namespace ty