#pragma once

namespace ty {

class MainEditor {
public:
	MainEditor() = default;
	~MainEditor() = default;

	void Update();

	void DrawWorkspace();

private:
	bool mShowDemoWindow = true;
	bool mShowWorkspace = true;

};

}  // namespace ty