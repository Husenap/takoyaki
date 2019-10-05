#pragma once

#include "../../ServiceManager.h"

namespace ty {
class ProjectSystem {
public:
	ProjectSystem(FileLocator& fileLocator);
	~ProjectSystem();

	void NewProject();
	void SaveProject();
	void OpenProject();

	bool IsProjectLoaded() const { return !mProjectFile.empty(); }

private:
	std::string mProjectFile;
	std::string mProjectDirectory;

	FileLocator& mFileLocator;

};
}  // namespace ty