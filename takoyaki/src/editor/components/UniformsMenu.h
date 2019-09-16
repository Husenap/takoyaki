#pragma once

#include "UniformItem.h"
#include "Widget.h"

namespace ty {

class UniformsMenu : public Widget {
public:
	void Update();

	void DrawUniforms();
	void DrawAddUniformPopup();

	void HandleNewUniform();

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

	UniformsChangedHandler mUniformsChangedHandler;

	// Create Uniform Popup Data
	std::array<char, 64> mNameBuffer;
	UniformType mSelectedType;
};

}  // namespace ty