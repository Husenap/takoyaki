#pragma once

#include "KeyFrameList.h"

namespace ty {

class AnimationTrack {

public:
	AnimationTrack(const std::string& name);
	~AnimationTrack();

	void DrawTick(int tick);
	void InsertKey(int tick);
	void RemoveKey(int tick);
	void ToggleEasingType(int tick);

	const std::string& GetName() const { return mName; }

private:
	std::string mName;
	KeyFrameList mKeys;
};
}  // namespace ty