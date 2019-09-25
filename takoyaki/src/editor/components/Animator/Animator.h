#pragma once

#include "../Widget.h"
#include "AnimationTrack.h"

namespace ty {

class Animator : public Widget {
public:
	Animator();
	~Animator();

	void Update();

	void OnInput(const KeyInput& input);

private:
	std::vector<AnimationTrack> mAnimationTracks;
	int mX = 0;
	int mY = 0;
	int mNumBars;
	int mNumBeats;
	int mNumTicks;
	void DrawAnimationTracks();
	void DrawTimeline();
};

}  // namespace ty