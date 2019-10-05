#include "ProjectSystem.h"

#include <serializer/Serializer.h>

#include "FileLocator.h"

namespace ty {

const char* ProjectFileExtensionFilter = "*.typroj";
const char* ProjectFileExtension       = ".typroj";

ProjectSystem::ProjectSystem(FileLocator& fileLocator)
    : mFileLocator(fileLocator) {}
ProjectSystem::~ProjectSystem() {}
void ProjectSystem::NewProject() {
	const char* fileToCreate = tinyfd_saveFileDialog("New Project", "", 1, &ProjectFileExtensionFilter, nullptr);
	if (fileToCreate) {
		std::filesystem::path projectFile(fileToCreate);
		projectFile.replace_extension(ProjectFileExtension);
		mProjectFile = projectFile.string();

		std::filesystem::path dir(fileToCreate);
		dir.remove_filename();
		mProjectDirectory = dir.string();
		std::filesystem::create_directory(mProjectDirectory + "shaders");

		mFileLocator.SetProjectPath(mProjectDirectory);

		SaveProject();
	}
}
void ProjectSystem::SaveProject() {
	dubu::FileBuffer projectFile(mProjectFile, dubu::FileBuffer::Mode::Write);
}
void ProjectSystem::OpenProject() {}
}  // namespace ty