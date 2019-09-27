#pragma once

namespace ty {

class SyncSystem {
public:
	SyncSystem();
	~SyncSystem();

	void SetBPM(float bpm);
	void SetBars(int bars);
	void SetOffset(float offset);

	int SecondsToTick(float seconds) const;
	float TickToSeconds(int tick) const;

	int NumTicks() const { return mNumTicks; }

private:
	int mNumBars          = 0;
	int mNumBeats         = 0;
	int mNumTicks         = 0;
	float mBPM            = 0.f;
	float mSecondsPerTick = 0.f;
	float mOffset         = 0.f;
};

}  // namespace ty