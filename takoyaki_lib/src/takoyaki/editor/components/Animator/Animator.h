#pragma once

#include "../Widget.h"

#include "../../ServiceManager.h"

namespace ty {

class Animator : public Widget {
public:
	Animator(MusicSystem& musicSystem, AnimationSystem& animationSystem, SyncSystem& syncSystem);
	~Animator();

	void Update();

	void OnInput(const KeyInput& input);

	void SyncMusicToTick();

private:
	void DrawAnimationTracks();
	void DrawTimeline();

	void MoveTrackIndex(int change);
	void MoveTickIndex(int change);

	int mTrackIndex = 0;
	int mTick       = 0;

	MusicSystem& mMusic;
	AnimationSystem& mAnimationSystem;
	SyncSystem& mSyncSystem;
};

}  // namespace ty