#include "KeyFrameList.h"

namespace ty {

KeyFrameList::KeyFrameList() {}

KeyFrameList::~KeyFrameList() {}

void KeyFrameList::InsertKey(int tick, float value, Easings::Type easingType) {
	if (IsTickFree(tick)) {
		KeyFrame keyFrame{tick, value, easingType};

		const auto pred = [](const KeyFrame& a, const KeyFrame& b) { return a.mTick < b.mTick; };
		auto it         = std::lower_bound(mKeys.begin(), mKeys.end(), keyFrame, pred);
		mKeys.insert(it, keyFrame);
	}
}

void KeyFrameList::RemoveKey(int tick) {
	const auto pred = [tick](const KeyFrame& k) { return k.mTick == tick; };
	auto it         = std::find_if(mKeys.begin(), mKeys.end(), pred);
	if (it != mKeys.end()) {
		mKeys.erase(it);
	}
}

void KeyFrameList::ToggleEasingType(int tick) {
	int startTick = FindStartTick(tick);

	if (startTick == -1) {
		return;
	}

	auto& key = mKeys[startTick];
	key.mEasingType = Easings::NextType(key.mEasingType);
}

float* KeyFrameList::GetTickValue(int tick) {
	const auto pred = [tick](const KeyFrame& key) { return tick == key.mTick; };
	auto it = std::find_if(mKeys.begin(), mKeys.end(), pred);
	if (it == mKeys.end()) {
		return nullptr;
	}
	return &it->mValue;
}

const KeyFramePair KeyFrameList::GetKeyPair(int tick) const {
	if (mKeys.empty()) {
		return {NullKey, NullKey};
	}

	int startIndex = FindStartTick(tick);

	int endIndex = std::min(startIndex + 1, (int)mKeys.size()-1);

	return {(startIndex == -1 ? NullKey : mKeys[startIndex]), (mKeys[endIndex])};
}

int KeyFrameList::FindStartTick(int tick) const {
	int startTick = -1;
	for (int i = 0; i < mKeys.size(); ++i) {
		if (mKeys[i].mTick <= tick) {
			startTick = i;
		} else {
			break;
		}
	}
	return startTick;
}

void KeyFrameList::Serialize(dubu::ReadBuffer& buffer) {
	buffer >> mKeys;
}

void KeyFrameList::Serialize(dubu::WriteBuffer& buffer) const {
	buffer << mKeys;
}

bool KeyFrameList::IsTickFree(int tick) {
	const auto pred = [tick](const KeyFrame& k) { return k.mTick == tick; };
	return std::none_of(mKeys.begin(), mKeys.end(), pred);
}

}  // namespace ty