#pragma once

namespace ty {

struct UniformItemFloat {
	const char* type = "float";
	float value;
	void Update(const char* name) { ImGui::DragFloat(name, &value, 0.1f); }
};
struct UniformItemVec2 {
	const char* type = "vec2";
	glm::vec2 value;
	void Update(const char* name) { ImGui::DragFloat2(name, &value.x, 0.1f); }
};
struct UniformItemVec3 {
	const char* type = "vec3";
	glm::vec3 value;
	void Update(const char* name) { ImGui::DragFloat3(name, &value.x, 0.1f); }
};
struct UniformItemVec4 {
	const char* type = "vec4";
	glm::vec4 value;
	void Update(const char* name) { ImGui::DragFloat4(name, &value.x, 0.1f); }
};

using UniformItem = std::variant<UniformItemFloat, UniformItemVec2, UniformItemVec3, UniformItemVec4>;

struct GetUniformType {
	template <typename T>
	std::string operator()(T& uniform) {
		return uniform.type;
	}
};

}  // namespace ty
