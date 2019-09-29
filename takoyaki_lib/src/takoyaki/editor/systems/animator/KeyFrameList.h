#pragma once

#include <serializer/Serializer.h>
#include "Easings.h"

namespace ty {

struct KeyFrame {
	int mTick;
	float mValue;
	Easings::Type mEasingType;
};
struct KeyFramePair {
	const KeyFrame& mStart;
	const KeyFrame& mEnd;
};

class KeyFrameList {
public:
	KeyFrameList();
	~KeyFrameList();

	void InsertKey(int tick, float value = 0.f, Easings::Type easingType = Easings::Type::Zero);
	void RemoveKey(int tick);

	const KeyFramePair GetKeyPair(int tick) const;
	int GetKeyCount() const { return (int)mKeys.size(); }

	void Serialize(dubu::ReadBuffer& buffer);
	void Serialize(dubu::WriteBuffer& buffer) const;

private:
	bool IsTickFree(int tick);

	std::vector<KeyFrame> mKeys;

	constexpr static KeyFrame NullKey{0, 0.f, Easings::Type::Zero};
};

}  // namespace ty