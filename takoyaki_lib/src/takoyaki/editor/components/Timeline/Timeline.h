#pragma once

#include "../Widget.h"

#include "../../ServiceManager.h"

namespace ty {
class Timeline : public Widget {
public:
	Timeline(MusicSystem& musicSystem);
	~Timeline();

	void Update();

	void DrawPlayPauseButton();

	void DrawVolumeControls();

	void DrawScrubber();

private:
	MusicSystem& mMusic;

	float mCurrentTime;

};
}  // namespace ty