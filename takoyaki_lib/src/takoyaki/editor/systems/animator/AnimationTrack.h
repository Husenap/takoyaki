#pragma once

#include "../../components/UniformItem.h"

namespace ty {

class AnimationTrack {
	/*
	struct KeyFrame {
		int mTick;
		UniformItem mData;
	};
	*/

public:
	AnimationTrack(const std::string& name, UniformType uniformType);
	~AnimationTrack();

	void DrawIndex(int index);
	void InsertKey(int index);

	const std::string& GetName() const { return mName; }

private:
	/*
	std::optional<KeyFrame&> FindKeyFrame(int tick) {
		const auto it = std::find_if(mVKeyFrames.begin(), mVKeyFrames.end(), [tick](const KeyFrame& keyFrame) {
			return keyFrame.mTick == tick;
		});

		if (it == mVKeyFrames.end()) {
			return std::nullopt;
		}

		return *it;
	}
	*/

	UniformType mUniformType;
	std::string mName;

	std::map<int, UniformItem> mKeyFrames;
	//std::vector<KeyFrame> mVKeyFrames;
};
}  // namespace ty