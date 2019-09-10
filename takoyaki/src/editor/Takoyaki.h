#pragma once

#include "../graphics/MainWindow.h"
#include "../graphics/Renderer.h"
#include "../graphics/ShaderProgram.h"
#include "MainEditor.h"

namespace ty {

class Takoyaki {
public:
	Takoyaki();
	~Takoyaki();

private:
	void OnInput(const KeyInput& input);
	void OnFramebufferSize(const glm::ivec2& size);
	void OnContentScale(const glm::vec2& scale);

	void ReloadShader();

	Renderer mRenderer;
	MainWindow mWindow;
	MainEditor mEditor;

	std::unique_ptr<ShaderProgram> mProgram;
	GLint vPosLocation;
	GLint iFrameLocation;
	GLint iTimeLocation;
	GLint iResolutionLocation;

	std::string mShaderFileToLoad;
};

}  // namespace ty