#include "AnimationSystem.h"

#include "../../systems/sync/SyncSystem.h"

namespace ty {

AnimationSystem::AnimationSystem(SyncSystem& syncSystem)
    : mSyncSystem(syncSystem) {
	mAnimationTracks.push_back(AnimationTrack("Scene", false));
	mAnimationTracks.push_back(AnimationTrack("Camera1"));
	mAnimationTracks.push_back(AnimationTrack("Camera2"));
}

AnimationSystem::~AnimationSystem() {}

std::vector<TrackValue> AnimationSystem::Evaluate(float time) {
	std::vector<TrackValue> trackValues;

	int tick = mSyncSystem.SecondsToTick(time);
	
	for (auto& track : mAnimationTracks) {
		const auto pair = track.GetKeyPair(tick);
		
		if (pair.mStart.mTick == pair.mEnd.mTick) {
			trackValues.emplace_back(TrackValue{pair.mStart.mValue, track.GetName()});
			continue;
		}
		
		float startTime = mSyncSystem.TickToSeconds(pair.mStart.mTick);
		float endTime = mSyncSystem.TickToSeconds(pair.mEnd.mTick);

		float delta = (time - startTime) / (endTime - startTime);
		float value = Easings::Ease(pair.mStart.mValue, pair.mEnd.mValue, delta, pair.mStart.mEasingType);
		trackValues.emplace_back(TrackValue{value, track.GetName()});
	}

	return trackValues;
}

}  // namespace ty