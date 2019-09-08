#pragma once

namespace ty {

class MainEditor {
public:
	MainEditor() = default;
	~MainEditor() = default;

	void Update();

	void DrawWorkspace();

	void OnInput(const KeyInput& input);
	void OnFramebufferSize(const glm::ivec2& size);

private:
	bool mShowDemoWindow = true;
	bool mShowWorkspace = true;

	ImVec2 mMenuBarSize;
	glm::ivec2 mFramebufferSize;
};

}  // namespace ty