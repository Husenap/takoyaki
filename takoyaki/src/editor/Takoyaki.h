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
	void OnInput(const KeyInput& input);
	void OnFramebufferSize(const glm::ivec2& size);

	Renderer mRenderer;
	MainWindow mWindow;
	MainEditor mEditor;
};

}  // namespace ty