#include "FileLocator.h"

namespace ty {

FileLocator::FileLocator() {}

FileLocator::~FileLocator() {}

const std::string FileLocator::Locate(const std::string& directoryPath) const {
	return mProjectPath + "/" + directoryPath;
}

void FileLocator::SetProjectPath(const std::string& projectPath) {
	mProjectPath = projectPath;
}

}  // namespace ty
