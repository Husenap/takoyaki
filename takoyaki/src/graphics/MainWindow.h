#pragma once

#include "BaseWindow.h"

namespace ty {

class MainWindow : public BaseWindow<MainWindow> {
public:
	MainWindow(int width, int height, const char* title);
	virtual ~MainWindow();

	virtual void OnInput(const KeyInput& input) override;
	virtual void OnInput(const MouseInput& input) override;
	virtual void OnInput(const CursorInput& input) override;
	virtual void OnFramebufferSize(const glm::ivec2& size) override;
	virtual void OnContentScale(const glm::vec2& size) override;

private:
	void InitGL();
	void InitImGui();
	void SetupImGuiStyle();

	struct FontData {
		float mScale;
		ImFont* mFont;
	};
	std::vector<FontData> mFonts;
	void UpdateScales();
};

}  // namespace ty