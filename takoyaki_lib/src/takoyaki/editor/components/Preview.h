#pragma once

#include "Widget.h"

namespace ty {

class Preview : public Widget {
public:
	void Update(const RenderTarget& renderTarget);

	const bool& IsHovered() const { return mIsHovered; }

private:
	bool mIsHovered;
};

}  // namespace ty