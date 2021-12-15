#pragma once

#include "Widget.h"

namespace ty {

class Exporter : public Widget {
public:
	void Update(RenderTarget& renderTarget);

	void Export();

private:
	void DoExport(RenderTarget& renderTarget);

	bool mShouldOpen = false;
	bool mShouldExportData = false;

	glm::ivec2 mSize;
	glm::ivec2 mOldSize;
};

}  // namespace ty
