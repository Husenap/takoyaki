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
#include <memory>
#include <filesystem>
#include <thread>
#include <mutex>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "util/glmvecToImVec.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <tinyfiledialogs.h>

#include "util/Input.h"
#include "util/VariantPolyfill.h"
#include "util/FileWatcher.h"

#include "graphics/gl/Shader.h"
#include "graphics/gl/ShaderProgram.h"
#include "graphics/gl/RenderCommand.h"
#include "graphics/gl/RenderTarget.h"

#include "editor/handlers/Handlers.h"
