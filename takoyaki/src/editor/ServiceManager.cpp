#include "ServiceManager.h"

#include "../graphics/MainWindow.h"
#include "../graphics/Renderer.h"
#include "MainEditor.h"
#include "Takoyaki.h"
#include "components/Animator/Animator.h"
#include "components/Camera.h"
#include "components/DockSpace.h"
#include "components/Preview.h"
#include "components/Timeline/Timeline.h"
#include "components/UniformsMenu.h"
#include "systems/music/MusicSystem.h"

namespace ty {

ServiceManager::ServiceManager() {
	mMusicManager = std::make_unique<ty::MusicSystem>();

	mWindow       = std::make_unique<ty::MainWindow>(1600, 900, "Takoyaki");
	mCamera       = std::make_unique<ty::Camera>();
	mPreview      = std::make_unique<ty::Preview>();
	mUniformsMenu = std::make_unique<ty::UniformsMenu>();
	mDockSpace    = std::make_unique<ty::DockSpace>();
	mAnimator     = std::make_unique<ty::Animator>(*mMusicManager);
	mTimeline     = std::make_unique<ty::Timeline>(*mMusicManager);
	mEditor =
	    std::make_unique<ty::MainEditor>(*mCamera, *mPreview, *mUniformsMenu, *mDockSpace, *mAnimator, *mTimeline);
	mFileWatcher = std::make_unique<ty::FileWatcher>();
	mRenderer    = std::make_unique<ty::Renderer>();

	mTakoyaki = std::make_unique<ty::Takoyaki>(*mWindow, *mRenderer, *mFileWatcher, *mEditor, *mCamera, *mUniformsMenu, *mMusicManager);
}
ServiceManager::~ServiceManager() {}

}  // namespace ty