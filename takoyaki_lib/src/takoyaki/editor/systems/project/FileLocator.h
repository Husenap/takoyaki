#pragma once

namespace ty {

class FileLocator {
	friend class ProjectSystem;

public:
	FileLocator();
	~FileLocator();

	const std::string Locate(const std::string& directoryPath) const;

private:
	void SetProjectPath(const std::string& projectPath);

	std::string mProjectPath;
};

}  // namespace ty