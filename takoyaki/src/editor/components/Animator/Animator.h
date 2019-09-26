#pragma once

#include "../Widget.h"
#include "AnimationTrack.h"

#include "../../ServiceManager.h"

namespace ty {

class Animator : public Widget {
public:
	Animator(MusicSystem& musicSystem);
	~Animator();

	void Update();

	void OnInput(const KeyInput& input);

	void SyncMusicToTick();

private:
	void DrawAnimationTracks();
	void DrawTimeline();
	int CalcTickFromSeconds(float seconds);
	float CalcSecondsFromTick(int tick);

	std::vector<AnimationTrack> mAnimationTracks;
	int mTrackIndex = 0;
	int mTick = 0;
	int mNumBars;
	int mNumBeats;
	int mNumTicks;
	float mBPM;
	float mSecondsPerTick;
	float mOffset;

	MusicSystem& mMusic;
};

}  // namespace ty