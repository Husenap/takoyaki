#include "FileWatcher.h"

namespace ty {
FileWatcher::FileWatcher() {
	mThread = std::thread([&]() { Job(); });
}
FileWatcher::~FileWatcher() {
	mIsRunning = false;
	if (mThread.joinable()) {
		mThread.join();
	}
}
void FileWatcher::Job() {
	mIsRunning = true;

	while (mIsRunning) {
		for (auto& [file, data] : mFiles) {
			std::error_code ec;
			auto writeTime = std::filesystem::last_write_time(file, ec);
			if (ec) {
				continue;
			}
			if (data.mLastWriteTime != writeTime) {
				data.mLastWriteTime = writeTime;
				std::lock_guard<std::mutex> lock(mMutex);
				mMarkedFiles.push_back(file);
			}
		}
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(100));
	}
}
void FileWatcher::Flush() {
	if (mMarkedFiles.empty()) return;

	std::lock_guard<std::mutex> lock(mMutex);

	for (const auto& file : mMarkedFiles) {
		auto data = mFiles.find(file);
		if (data != mFiles.end()) {
			data->second.mCallback(file);
		}
	}
	mMarkedFiles.clear();
}
void FileWatcher::Watch(const std::string& file, FileChangeCallback callback) {
	FileData data;
	data.mFilePath      = file;
	data.mCallback      = callback;
	data.mLastWriteTime = std::filesystem::last_write_time(file);
	mFiles[file]        = data;
}
void FileWatcher::Clear() {
	mFiles.clear();
}
}  // namespace ty