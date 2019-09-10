#pragma once

namespace ty {

class MainEditor {
public:
	MainEditor() = default;
	~MainEditor() = default;

	void Update();

	void DrawUniformList();

	void OnInput(const KeyInput& input);
	void OnFramebufferSize(const glm::ivec2& size);
	void OnContentScale(const glm::vec2& scale);

private:
	bool mShowDemoWindow = true;
	bool mShowWorkspace = true;

	ImVec2 mMenuBarSize;
	glm::ivec2 mFramebufferSize;
	std::vector<std::string> mUniforms;
};

}  // namespace ty