#pragma once

namespace ty {

class Preview {
public:

	void Update(const std::unique_ptr<RenderTarget>& renderTarget);

	const bool& IsHovered() const { return mIsHovered; }

private:
	bool mIsHovered;
};

}  // namespace ty