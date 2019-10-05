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
#include "systems/animator/AnimationSystem.h"
#include "systems/music/MusicSystem.h"
#include "systems/project/FileLocator.h"
#include "systems/project/ProjectSystem.h"
#include "systems/scene/SceneSystem.h"
#include "systems/sync/SyncSystem.h"

namespace ty {

ServiceManager::ServiceManager() {
	mFileLocator   = std::make_unique<ty::FileLocator>();
	mProjectSystem = std::make_unique<ty::ProjectSystem>(*mFileLocator);
	mSceneSystem   = std::make_unique<ty::SceneSystem>(*mFileLocator);

	mSyncSystem      = std::make_unique<ty::SyncSystem>();
	mMusicSystem     = std::make_unique<ty::MusicSystem>();
	mAnimationSystem = std::make_unique<ty::AnimationSystem>(*mSyncSystem);

	mWindow      = std::make_unique<ty::MainWindow>(1600, 900, "Takoyaki");
	mCamera      = std::make_unique<ty::Camera>();
	mPreview     = std::make_unique<ty::Preview>();
	mDockSpace   = std::make_unique<ty::DockSpace>();
	mFileWatcher = std::make_unique<ty::FileWatcher>();
	mRenderer    = std::make_unique<ty::Renderer>();
	mAnimator    = std::make_unique<ty::Animator>(*mMusicSystem, *mAnimationSystem, *mSyncSystem);
	mTimeline    = std::make_unique<ty::Timeline>(*mMusicSystem);
	mEditor =
	    std::make_unique<ty::MainEditor>(*mCamera, *mPreview, *mDockSpace, *mAnimator, *mTimeline, *mProjectSystem, *mSceneSystem);

	mTakoyaki = std::make_unique<ty::Takoyaki>(
	    *mWindow, *mRenderer, *mFileWatcher, *mEditor, *mCamera, *mMusicSystem, *mAnimationSystem);
}
ServiceManager::~ServiceManager() {}

}  // namespace ty