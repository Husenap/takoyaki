#include "SyncSystem.h"

namespace ty {

SyncSystem::SyncSystem() {}

SyncSystem::~SyncSystem() {}

void SyncSystem::SetBPM(float bpm) {
	mBPM            = bpm;
	mSecondsPerTick = (60.f / (mBPM * 4.f));
}

void SyncSystem::SetBars(int bars) {
	mNumBars  = bars;
	mNumBeats = mNumBars * 4;
	mNumTicks = mNumBeats * 4;
}

void SyncSystem::SetOffset(float offset) {
	mOffset = offset;
}

int SyncSystem::SecondsToTick(float seconds) const {
	return (int)std::roundf((seconds - mOffset) / mSecondsPerTick);
}

float SyncSystem::TickToSeconds(int tick) const {
	return ((float)tick * mSecondsPerTick) + mOffset;
}

float SyncSystem::TickToSecondsWithoutOffset(int tick) const {
	return (float)tick * mSecondsPerTick;
}

}  // namespace ty