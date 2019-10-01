#pragma once

namespace ty {
class MusicSystem {
	static const int NumPeaks = 512;

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
	const std::array<float, NumPeaks>& GetPeaks() const { return mPeaks; }

private:
	unsigned long LoadChannelFromFile(const std::string& filepath, unsigned long streamFlags, unsigned long musicFlags);
	void LoadSpectrum(unsigned long decoder);

	unsigned long mChannel;
	float mLengthSeconds;
	uint64_t mLengthBytes;
	std::array<float, NumPeaks> mPeaks;
};

}  // namespace ty