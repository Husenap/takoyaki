#pragma once

namespace ty {
class MusicSystem {
public:
	MusicSystem();
	~MusicSystem();

	bool LoadMusic(const std::string& filepath);
	void UnloadMusic();
	void Play(float seconds = 0.f);
	void Pause();
	void Seek(float seconds);
	void SetVolume(float volume);

	bool IsLoaded() const { return mChannel != 0; }
	bool IsPlaying() const;
	float GetLengthSeconds() const { return mLengthSeconds; }
	float GetCurrentPosition() const;

private:
	unsigned long mChannel;
	float mLengthSeconds;
	uint64_t mLengthBytes;
};

}  // namespace ty