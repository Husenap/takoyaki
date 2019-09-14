#pragma once

namespace ty {

struct UniformItemFloat {
	float value;
	void Update(const char* name) { ImGui::DragFloat(name, &value, 0.1f); }
	static constexpr const char* type() { return "float"; }
};
struct UniformItemVec2 {
	glm::vec2 value;
	void Update(const char* name) { ImGui::DragFloat2(name, &value.x, 0.1f); }
	static constexpr const char* type() { return "vec2"; }
};
struct UniformItemVec3 {
	glm::vec3 value;
	void Update(const char* name) { ImGui::DragFloat3(name, &value.x, 0.1f); }
	static constexpr const char* type() { return "vec3"; }
};
struct UniformItemVec4 {
	glm::vec4 value;
	void Update(const char* name) { ImGui::DragFloat4(name, &value.x, 0.1f); }
	static constexpr const char* type() { return "vec4"; }
};

struct UniformColor {
	glm::vec4 value;
	void Update(const char* name) { ImGui::ColorEdit4(name, &value.x, ImGuiColorEditFlags_HDR |ImGuiColorEditFlags_Float); }
	static constexpr const char* type() { return "vec4"; }
};

using UniformItem = std::variant<UniformItemFloat, UniformItemVec2, UniformItemVec3, UniformItemVec4, UniformColor>;

struct GetUniformType {
	template <typename T>
	std::string operator()(T& uniform) {
		return uniform.type();
	}
};

}  // namespace ty
