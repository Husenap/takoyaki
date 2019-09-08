#pragma once

#include "../graphics/MainWindow.h"
#include "../graphics/Renderer.h"
#include "MainEditor.h"

namespace ty {

class Takoyaki {
public:
	Takoyaki();
	~Takoyaki();

private:
	Renderer mRenderer;
	MainWindow mWindow;
	MainEditor mEditor;
};

}  // namespace ty