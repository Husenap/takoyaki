#pragma once

#include "Widget.h"

namespace ty {

class Preview : public Widget {
public:
	void Update(const std::unique_ptr<RenderTarget>& renderTarget);

	const bool& IsHovered() const { return mIsHovered; }

private:
	bool mIsHovered;
};

}  // namespace ty