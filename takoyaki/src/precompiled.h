#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <array>
#include <fstream>
#include <functional>
#include <optional>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "util/glmvecToImVec.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <tinyfiledialogs.h>

#include "util/KeyInput.h"
#include "util/VariantPolyfill.h"

#include "graphics/Shader.h"
#include "graphics/ShaderProgram.h"
#include "graphics/gl/RenderCommand.h"
