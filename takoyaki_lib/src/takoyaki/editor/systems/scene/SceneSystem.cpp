#include "SceneSystem.h"

#include "../project/FileLocator.h" j

namespace {
static const char* vertexShaderCode = R"(
#version 450
in vec2 vPos;
void main(){
	gl_Position = vec4(vPos, 1.0, 1.0);
}
)";

static const char* fragmentShaderCodeBegin = R"(
#version 450
out vec4 frag_color;
uniform float iTime;
uniform vec2 iResolution;
uniform int iFrame;
uniform vec3 iCameraOrigin;
uniform vec3 iCameraTarget;
)";

static const char* fragmentShaderCodeEnd = R"(
void main(){
	mainImage(frag_color, gl_FragCoord.xy);
}
)";

static const char* templateShaderCode = R"(// Available uniforms
//#=============================#
//* uniform float iTime;        *
//* uniform vec2 iResolution;   *
//* uniform int iFrame;         *
//* uniform vec3 iCameraOrigin; *
//* uniform vec3 iCameraTarget; *
//#=============================#

void mainImage(out vec4 fragColor, in vec2 fragCoord) { 
    vec2 uv = (fragCoord*2.0 - iResolution.xy) / iResolution.y;

    vec3 col = vec3(0.0);
	col.rg = uv.xy;

    fragColor = vec4(col, 1.0);
}
)";

static const char* ShaderFileTypeFilter = "*.glsl";
}  // namespace

namespace ty {

SceneSystem::SceneSystem(FileLocator& fileLocator)
    : mFileLocator(fileLocator) {}

SceneSystem::~SceneSystem() {}

void SceneSystem::NewScene() {
	const char* fileToCreate = tinyfd_saveFileDialog("New Scene", "", 1, &ShaderFileTypeFilter, nullptr);

	if (fileToCreate) {
		std::ofstream f(fileToCreate, std::ios_base::trunc);
		if (!f.is_open()) {
			tinyfd_notifyPopup("Error!", "Failed to create new scene file!", "error");
			return;
		}

		f << templateShaderCode;
		f.close();

		AddScene(fileToCreate);
	}
}

void SceneSystem::AddScene(const std::string& filepath) {
	const auto pred = [filepath](const SceneData& scene) { return scene.mFilepath == filepath; };
	if (std::any_of(mScenes.begin(), mScenes.end(), pred)) {
		return;
	}

	LoadScene(filepath);
}

void SceneSystem::RemoveScene(int index) {
	if (index >= 0 && index < mScenes.size()) {
		mScenes.erase(mScenes.begin() + index);
	}
}

void SceneSystem::LoadScene(const std::string& filepath) {
	SceneData newScene;
	newScene.mFilepath = filepath;
	newScene.mProgram  = std::make_unique<ShaderProgram>();
	mScenes.emplace_back(SceneData{filepath, LoadShader(filepath)});
}

std::unique_ptr<ShaderProgram> SceneSystem::LoadShader(const std::string& filepath) {
	std::ifstream shaderFile(filepath);
	std::string shaderFileCode((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();
	std::string shaderCode = fragmentShaderCodeBegin;
	shaderCode += "#line 1\n" + shaderFileCode;
	shaderCode += fragmentShaderCodeEnd;

	VertexShader vShader(vertexShaderCode);
	FragmentShader fShader(shaderCode.c_str());

	auto program = std::make_unique<ShaderProgram>(vShader, fShader);

	auto error = program->GetError();
	if (error.has_value()) {
		tinyfd_notifyPopup("Error!", error.value().c_str(), "error");
	}

	return program;
}

}  // namespace ty