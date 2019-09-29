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
#include "systems/animator/AnimationSystem.h"
#include "systems/music/MusicSystem.h"
#include "systems/sync/SyncSystem.h"

namespace ty {

ServiceManager::ServiceManager() {
	mSyncSystem      = std::make_unique<ty::SyncSystem>();
	mMusicSystem     = std::make_unique<ty::MusicSystem>();
	mAnimationSystem = std::make_unique<ty::AnimationSystem>();

	mWindow       = std::make_unique<ty::MainWindow>(1600, 900, "Takoyaki");
	mCamera       = std::make_unique<ty::Camera>();
	mPreview      = std::make_unique<ty::Preview>();
	mUniformsMenu = std::make_unique<ty::UniformsMenu>();
	mDockSpace    = std::make_unique<ty::DockSpace>();
	mAnimator     = std::make_unique<ty::Animator>(*mMusicSystem, *mAnimationSystem, *mSyncSystem);
	mTimeline     = std::make_unique<ty::Timeline>(*mMusicSystem);
	mEditor =
	    std::make_unique<ty::MainEditor>(*mCamera, *mPreview, *mUniformsMenu, *mDockSpace, *mAnimator, *mTimeline);
	mFileWatcher = std::make_unique<ty::FileWatcher>();
	mRenderer    = std::make_unique<ty::Renderer>();

	mTakoyaki = std::make_unique<ty::Takoyaki>(
	    *mWindow, *mRenderer, *mFileWatcher, *mEditor, *mCamera, *mUniformsMenu, *mMusicSystem);
}
ServiceManager::~ServiceManager() {}

}  // namespace ty