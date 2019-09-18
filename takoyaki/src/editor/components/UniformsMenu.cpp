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
const char* UniformDragAndDropName = "REORDER_UNIFORMS_DRAG_AND_DROP";

void UniformsMenu::Update() {
	size_t numUniforms = mUniforms.size();

	if (mVisibility) {
		if (ImGui::Begin(UniformsName, &mVisibility)) {
			DrawUniforms();

			DrawAddUniformPopup();
			ImGui::End();
		}
	}

	if (numUniforms != mUniforms.size()) {
		if (mUniformsChangedHandler) mUniformsChangedHandler();
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

	SwapData uniformSwap{-1, -1};
	for (int i = 0; i < mUniforms.size();) {
		ImGui::PushID(i);

		auto& uniform = mUniforms[i];

		ImGui::BeginGroup();
		bool wasButtonPressed = ImGui::Button("X");
		ImGui::SameLine();
		std::visit(make_overloaded{[](auto& item) { item.Update(); }}, uniform.mItem);
		ImGui::SameLine();

		ImGui::Text(uniform.mName.c_str());

		const static int dragDropFlags = ImGuiDragDropFlags_SourceAllowNullID | ImGuiDragDropFlags_AcceptBeforeDelivery;
		if (ImGui::BeginDragDropSource(dragDropFlags)) {
			ImGui::SetDragDropPayload(UniformDragAndDropName, &i, sizeof(int));
			ImGui::Text(uniform.mName.c_str());
			ImGui::EndDragDropSource();
		}

		ImGui::EndGroup();

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(UniformDragAndDropName)) {
				int payload_index = *(const int*)payload->Data;
				uniformSwap = {payload_index, i};
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();

		if (wasButtonPressed) {
			mUniforms.erase(mUniforms.begin() + i);
		} else {
			++i;
		}
	}

	if (uniformSwap.mSourceIndex != -1 && uniformSwap.mTargetIndex != -1) {
		UniformData data = mUniforms[uniformSwap.mSourceIndex];
		mUniforms.erase(mUniforms.begin() + uniformSwap.mSourceIndex);
		mUniforms.insert(mUniforms.begin() + uniformSwap.mTargetIndex, data);
	}
}

void UniformsMenu::DrawAddUniformPopup() {
	if (ImGui::BeginPopupModal(AddNewUniformPopupName, nullptr, ImGuiWindowFlags_NoResize)) {
		ImGui::SetWindowSize({0.f, 0.f});
		ImGui::Combo("Type", (int*)&mSelectedType, "float\0vec2\0vec3\0vec4\0color\0\0");

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

	auto pred = [& n = name](const UniformData& data) { return n == data.mName; };
	if (std::any_of(mUniforms.begin(), mUniforms.end(), pred)) {
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
	case UniformType::Color:
		mUniforms.push_back({name.data(), UniformColor{}});
		break;
	}
}

void UniformsMenu::RegisterCommands(RenderCommandList<RenderCommand>& cmds, const ShaderProgram& program) {
	for (auto& uniform : mUniforms) {
		auto pred = [&](auto& item) { cmds.Push<Commands::Uniform>(program.mProgram, uniform.mName, item.value); };
		std::visit(make_overloaded{pred}, uniform.mItem);
	}
}

void UniformsMenu::OpenFile(std::string_view file) {
	mUniforms.clear();

	std::ifstream f(std::string(file) + ".uniforms", std::ios_base::binary);
	if (!f.is_open()) return;

	unsigned char numUniforms;
	f.read((char*)&numUniforms, sizeof(numUniforms));

	mUniforms.resize(numUniforms);
	for (size_t i = 0; i < numUniforms; ++i) {
		f.read((char*)&mUniforms[i].mItem, sizeof(mUniforms[i].mItem));
	}

	for (size_t i = 0; i < numUniforms; ++i) {
		std::getline(f, mUniforms[i].mName);
	}

	f.close();
}

void UniformsMenu::SaveFile(std::string_view file) {
	std::ofstream f(std::string(file) + ".uniforms", std::ios_base::binary);
	if (!f.is_open()) return;

	unsigned char numUniforms = (unsigned char)mUniforms.size();
	f.write((char*)&numUniforms, sizeof(numUniforms));

	for (const auto& uniform : mUniforms) {
		f.write((char*)&uniform.mItem, sizeof(uniform.mItem));
	}

	for (const auto& uniform : mUniforms) {
		f << uniform.mName << std::endl;
	}

	f.close();
}

std::string UniformsMenu::GetUniformDeclarations() {
	std::string output = "";

	for (const auto& uniform : mUniforms) {
		output += "uniform ";
		output += std::visit(GetUniformType{}, uniform.mItem);
		output += " ";
		output += uniform.mName;
		output += ";\n";
	}

	return output;
}

}  // namespace ty
