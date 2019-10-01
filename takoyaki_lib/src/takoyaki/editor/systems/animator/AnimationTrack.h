#pragma once

#include "KeyFrameList.h"

namespace ty {

class AnimationTrack {
public:
	AnimationTrack(const std::string& name, bool isUserTrack = true);
	~AnimationTrack();

	void DrawTick(int tick);
	void InsertKey(int tick);
	void RemoveKey(int tick);
	void ToggleEasingType(int tick);
	void FocusTick(int tick);

	const std::string& GetName() const { return mName; }
	const KeyFramePair GetKeyPair(int tick);
	const bool IsUserTrack() const { return mIsUserTrack; }

private:
	std::string mName;
	KeyFrameList mKeys;
	int mTickToFocus = -1;
	bool mIsUserTrack;
};
}  // namespace ty