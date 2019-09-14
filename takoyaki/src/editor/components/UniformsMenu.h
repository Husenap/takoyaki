#pragma once

#include "UniformItem.h"

namespace ty {

class UniformsMenu {
public:
	void Update();

	void DrawUniforms();
	void DrawAddUniformPopup();

	void HandleNewUniform();

	void ToggleVisibility();
	void RegisterCommands(RenderCommandList<RenderCommand>& cmds, std::unique_ptr<ShaderProgram>& program);

	void OpenFile(std::string_view file);
	void SaveFile(std::string_view file);

	void SetUniformsChangedHandler(UniformsChangedHandler handler) { mUniformsChangedHandler = handler; }

	std::string GetUniformDeclarations();

private:
	enum class UniformType : int { Float, Vec2, Vec3, Vec4, Color };
	struct UniformData {
		std::string mName;
		UniformItem mItem;
	};

	std::vector<UniformData> mUniforms;
	bool mVisibility;

	UniformsChangedHandler mUniformsChangedHandler;

	// Create Uniform Popup Data
	std::array<char, 64> mNameBuffer;
	UniformType mSelectedType;
};

}  // namespace ty