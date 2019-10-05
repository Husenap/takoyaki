#pragma once

#include "../../ServiceManager.h"

namespace ty {

class SceneSystem {
	struct SceneData {
		std::string mFilepath;
		std::unique_ptr<ShaderProgram> mProgram;
	};

public:
	SceneSystem(FileLocator& fileLocator);
	~SceneSystem();

	void NewScene();
	void AddScene(const std::string& filepath);
	void RemoveScene(int index);

private:
	void LoadScene(const std::string& filepath);
	std::unique_ptr<ShaderProgram> LoadShader(const std::string& filepath);

	std::vector<SceneData> mScenes;

	FileLocator& mFileLocator;
};

}  // namespace ty