#include "MusicSystem.h"

#include "bass.h"

namespace {
std::string GetBassErrorTitle() {
	return "Error[" + std::to_string(BASS_ErrorGetCode()) + "]";
}
}  // namespace

namespace ty {

MusicSystem::MusicSystem() {
	mChannel = 0;
	mLengthBytes = 0;
	mLengthSeconds = 0.f;

	if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
		tinyfd_notifyPopup("Error", "BASS version doesn't match the DLL version!", "error");
	}

	if (!BASS_Init(-1, 44100, 0, NULL, NULL)) {
		tinyfd_notifyPopup("Error", "Bass failed to Init!", "error");
	}
}

MusicSystem::~MusicSystem() {
	UnloadMusic();
	BASS_Free();
}

bool MusicSystem::LoadMusic(const std::string& filepath) {
	UnloadMusic();

	mChannel = BASS_StreamCreateFile(FALSE, filepath.c_str(), 0, 0, 0);
	if (!mChannel) {
		std::string title = GetBassErrorTitle();
		tinyfd_notifyPopup(title.c_str(), "Bass failed to Create Stream File!", "error");
		return false;
	}

	unsigned long musicFlags = BASS_MUSIC_PRESCAN | BASS_MUSIC_RAMP;
	mChannel                 = BASS_MusicLoad(FALSE, filepath.c_str(), 0, 0, BASS_MUSIC_RAMP, 1);
	if (!mChannel) {
		mChannel = BASS_SampleLoad(FALSE, filepath.c_str(), 0, 0, 1, BASS_MUSIC_RAMP);
	}

	if (!mChannel) {
		std::string title = GetBassErrorTitle();
		tinyfd_notifyPopup(title.c_str(), "Bass failed to Load Music!", "error");
		return false;
	}

	BASS_SampleGetChannel(mChannel, FALSE);

	mLengthBytes   = BASS_ChannelGetLength(mChannel, BASS_POS_BYTE);
	mLengthSeconds = (float)BASS_ChannelBytes2Seconds(mChannel, mLengthBytes);

	return true;
}

void MusicSystem::UnloadMusic() {
	BASS_SampleFree(mChannel);
	BASS_MusicFree(mChannel);
	mChannel = 0;
	mLengthBytes = 0;
	mLengthSeconds = 0.f;
}

void MusicSystem::Play(float seconds) {
	if (!mChannel) return;

	BASS_ChannelPlay(mChannel, FALSE);
}

void MusicSystem::Pause() {
	if (!mChannel) return;

	BASS_ChannelPause(mChannel);
}

void MusicSystem::Seek(float seconds) {
	if (!mChannel) return;

	uint64_t bytePos = BASS_ChannelSeconds2Bytes(mChannel, (double)seconds);
	BASS_ChannelSetPosition(mChannel, bytePos, BASS_POS_BYTE);
}

void MusicSystem::SetVolume(float volume) {
	BASS_ChannelSetAttribute(mChannel, BASS_ATTRIB_VOL, std::clamp(volume, 0.f, 1.f));
}

bool MusicSystem::IsPlaying() const {
	unsigned long status = BASS_ChannelIsActive(mChannel);
	return status == BASS_ACTIVE_PLAYING;
}

float MusicSystem::GetCurrentPosition() const {
	if (!mChannel) return 0.0f;

	uint64_t bytePos = BASS_ChannelGetPosition(mChannel, BASS_POS_BYTE);
	float pos = (float)BASS_ChannelBytes2Seconds(mChannel, bytePos);
	return pos;
}

}  // namespace ty