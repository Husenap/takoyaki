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

	std::string GetUniformDeclarations();

private:
	enum class UniformType : int { Float, Vec2, Vec3, Vec4 };
	struct UniformData {
		std::string mName;
		UniformItem mItem;
	};

	std::vector<UniformData> mUniforms;

	std::array<char, 64> mNameBuffer;
	UniformType mSelectedType;

	bool mVisibility;
};

}  // namespace ty