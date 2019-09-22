#include "ServiceManager.h"

#include "../graphics/MainWindow.h"
#include "../graphics/Renderer.h"
#include "MainEditor.h"
#include "Takoyaki.h"
#include "components/Camera.h"

namespace ty {

ServiceManager::ServiceManager() {
	mWindow       = std::make_unique<ty::MainWindow>(1024, 768, "Takoyaki");
	mCamera       = std::make_unique<ty::Camera>();
	mPreview      = std::make_unique<ty::Preview>();
	mUniformsMenu = std::make_unique<ty::UniformsMenu>();
	mEditor       = std::make_unique<ty::MainEditor>(*mCamera, *mPreview, *mUniformsMenu);
	mFileWatcher  = std::make_unique<ty::FileWatcher>();
	mRenderer     = std::make_unique<ty::Renderer>();

	mTakoyaki = std::make_unique<ty::Takoyaki>(*mWindow, *mRenderer, *mFileWatcher, *mEditor, *mCamera, *mUniformsMenu);
}
ServiceManager::~ServiceManager() {}

}  // namespace ty