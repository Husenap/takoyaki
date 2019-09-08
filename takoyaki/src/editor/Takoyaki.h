#pragma once

#include "../graphics/MainWindow.h"
#include "../graphics/Renderer.h"

namespace ty {

class Takoyaki {
public:
	Takoyaki();
	~Takoyaki();

private:
	Renderer mRenderer;
	MainWindow mWindow;
};

}  // namespace ty