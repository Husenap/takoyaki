#pragma once

#include "../UniformItem.h"

namespace ty {

class AnimationTrack {
public:
	AnimationTrack(const std::string& name, UniformType uniformType);
	~AnimationTrack();

	void DrawIndex(int index);
	void InsertKey(int index);

	const std::string& GetName() const { return mName; }

private:
	UniformType mUniformType;
	std::string mName;

	std::map<int, UniformItem> mKeyFrames;
};
}  // namespace ty