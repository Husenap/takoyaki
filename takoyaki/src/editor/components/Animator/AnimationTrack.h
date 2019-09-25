#pragma once

namespace ty {
class AnimationTrack {
public:
	AnimationTrack(const std::string& name);
	~AnimationTrack();

	const std::string& GetName() const { return mName; }

private:
	std::string mName;
};
}  // namespace ty