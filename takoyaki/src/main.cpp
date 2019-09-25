#include "editor/Takoyaki.h"

#include "editor/ServiceManager.h"

//#include "bass.h"

int main() {
	/*
	if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
		tinyfd_notifyPopup("Error", "Bass version doesn't match the DLL!", "error");
	}

	if (!BASS_Init(-1, 44100, 0, NULL, NULL)) {
		tinyfd_notifyPopup("Error", "Bass failed to Init!", "error");
	}

	const char* filter     = "*.*";
	const char* fileToLoad = tinyfd_openFileDialog("Choose a soundtrack", "", 1, &filter, nullptr, 0);
	if (fileToLoad) {
		DWORD chan;
		if (!(chan = BASS_StreamCreateFile(FALSE, fileToLoad, 0, 0, BASS_SAMPLE_LOOP))) {
			tinyfd_messageBox("Error", "Failed to Create stream", "ok", "error", 1);
		}
		if (!(chan = BASS_SampleLoad(FALSE, fileToLoad, 0, 0, 1, BASS_MUSIC_RAMP))) {
			tinyfd_messageBox("Error", "Failed to Load music", "ok", "error", 1);
			int errorCode = BASS_ErrorGetCode();
			std::cout << errorCode << std::endl;
		}
		BASS_SampleGetChannel(chan, FALSE);
		BASS_ChannelPlay(chan, FALSE);
	}
	*/

	ty::ServiceManager serviceManager;

	//BASS_Free();

	return 0;
}