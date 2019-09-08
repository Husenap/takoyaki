#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include <experimental/filesystem>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace ty {

template <class... Ts>
struct make_overloaded : Ts... {
	using Ts::operator()...;
};
template <class... Ts>
make_overloaded(Ts...)->make_overloaded<Ts...>;

}  // namespace ty
