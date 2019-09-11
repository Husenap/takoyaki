#include "UniformsMenu.h"

namespace {
static int CharacterFilter(ImGuiTextEditCallbackData* data) {
	ImWchar c    = data->EventChar;
	int numChars = (*(int*)data->UserData);
	if (std::isalpha(c)) return 0;
	if (std::isdigit(c) && numChars > 0) return 0;
	if (c == '_') return 0;
	return 1;
}
}  // namespace

namespace ty {

const char* UniformsName           = "Uniforms";
const char* AddNewUniformPopupName = "AddNewUniform";

void UniformsMenu::Update() {
	if (!mVisibility) {
		return;
	}

	if (ImGui::Begin(UniformsName, &mVisibility)) {
		DrawUniforms();

		DrawAddUniformPopup();
		ImGui::End();
	}
}

void UniformsMenu::DrawUniforms() {
	if (ImGui::Button("Add Uniform")) {
		if (!ImGui::IsPopupOpen(AddNewUniformPopupName)) {
			mNameBuffer.fill('\0');
			mSelectedType = UniformType::Float;
			ImGui::OpenPopup(AddNewUniformPopupName);
		}
	}

	auto pred = [](auto& uniform) {
		bool wasButtonPressed = ImGui::Button(("X##" + uniform.mName).c_str());
		ImGui::SameLine();
		std::visit(make_overloaded{[&](auto& item) { item.Update(uniform.mName.c_str()); }}, uniform.mItem);
		return wasButtonPressed;
	};
	mUniforms.erase(std::remove_if(mUniforms.begin(), mUniforms.end(), pred), mUniforms.end());
}

void UniformsMenu::DrawAddUniformPopup() {
	if (ImGui::BeginPopupModal(AddNewUniformPopupName, nullptr, ImGuiWindowFlags_NoResize)) {
		ImGui::SetWindowSize({0.f, 0.f});
		ImGui::Combo("Type", (int*)&mSelectedType, "float\0vec2\0vec3\0vec4\0\0");

		int numChars = static_cast<int>(std::string(mNameBuffer.data()).size());
		ImGui::InputText("Variable Name",
		                 mNameBuffer.data(),
		                 mNameBuffer.size(),
		                 ImGuiInputTextFlags_CallbackCharFilter,
		                 CharacterFilter,
		                 &numChars);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.f, 0.f));
		ImGui::Columns(2, nullptr, false);
		if (ImGui::Button("Create", {-FLT_EPSILON, 0.0})) {
			HandleNewUniform();
			ImGui::CloseCurrentPopup();
		}
		ImGui::NextColumn();
		if (ImGui::Button("Cancel", {-FLT_EPSILON, 0.0})) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::Columns(1);
		ImGui::PopStyleVar();

		ImGui::EndPopup();
	}
}

void UniformsMenu::HandleNewUniform() {
	std::string name = mNameBuffer.data();
	if (name.empty()) {
		return;
	}

	switch (mSelectedType) {
	case UniformType::Float:
		mUniforms.push_back({name.data(), UniformItemFloat{}});
		break;
	case UniformType::Vec2:
		mUniforms.push_back({name.data(), UniformItemVec2{}});
		break;
	case UniformType::Vec3:
		mUniforms.push_back({name.data(), UniformItemVec3{}});
		break;
	case UniformType::Vec4:
		mUniforms.push_back({name.data(), UniformItemVec4{}});
		break;
	}
}

void UniformsMenu::ToggleVisibility() {
	mVisibility = !mVisibility;
}

void UniformsMenu::RegisterCommands(RenderCommandList<RenderCommand>& cmds, std::unique_ptr<ShaderProgram>& program) {
	for (auto& uniform : mUniforms) {
		auto pred = [&](auto& item) {
			cmds.Push<Commands::Uniform>(program->mProgram, uniform.mName, item.value);
		};
		std::visit(make_overloaded{pred}, uniform.mItem);
	}
}

}  // namespace ty
