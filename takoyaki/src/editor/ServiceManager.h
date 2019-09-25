#pragma once

namespace ty {
class Renderer;
class FileWatcher;
class MainWindow;
class MainEditor;
class Takoyaki;
class Camera;
class UniformsMenu;
class Preview;
class DockSpace;
class Animator;
class Timeline;
class MusicSystem;
}  // namespace ty

namespace ty {

class ServiceManager {
public:
	ServiceManager();
	~ServiceManager();

private:
	std::unique_ptr<ty::MainWindow> mWindow;
	std::unique_ptr<ty::MainEditor> mEditor;
	std::unique_ptr<ty::FileWatcher> mFileWatcher;
	std::unique_ptr<ty::Renderer> mRenderer;
	std::unique_ptr<ty::Camera> mCamera;
	std::unique_ptr<ty::Preview> mPreview;
	std::unique_ptr<ty::DockSpace> mDockSpace;
	std::unique_ptr<ty::UniformsMenu> mUniformsMenu;
	std::unique_ptr<ty::Animator> mAnimator;
	std::unique_ptr<ty::Timeline> mTimeline;

	std::unique_ptr<ty::MusicSystem> mMusicManager;

	std::unique_ptr<ty::Takoyaki> mTakoyaki;
};

}  // namespace ty