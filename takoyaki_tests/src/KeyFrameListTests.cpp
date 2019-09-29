#include <gtest/gtest.h>

#include <serializer/Serializer.h>
#include <takoyaki/editor/systems/animator/KeyFrameList.h>

TEST(KeyFrameList, InsertKeyFrame) {
	ty::KeyFrameList list;
	EXPECT_EQ(list.GetKeyCount(), 0);
	list.InsertKey(4, 4.f);
	EXPECT_EQ(list.GetKeyCount(), 1);
	list.InsertKey(4, 4.f);
	EXPECT_EQ(list.GetKeyCount(), 1);
	list.InsertKey(0, 0.f);
	EXPECT_EQ(list.GetKeyCount(), 2);
	list.InsertKey(3, 3.f);
	EXPECT_EQ(list.GetKeyCount(), 3);
	list.InsertKey(1, 1.f);
	EXPECT_EQ(list.GetKeyCount(), 4);
}

TEST(KeyFrameList, RemoveKeyFrame) {
	ty::KeyFrameList list;
	EXPECT_EQ(list.GetKeyCount(), 0);
	list.InsertKey(4, 4.f);
	EXPECT_EQ(list.GetKeyCount(), 1);
	list.InsertKey(3, 4.f);
	EXPECT_EQ(list.GetKeyCount(), 2);
	list.InsertKey(0, 0.f);
	EXPECT_EQ(list.GetKeyCount(), 3);
	list.RemoveKey(1);
	EXPECT_EQ(list.GetKeyCount(), 3);
	list.RemoveKey(3);
	EXPECT_EQ(list.GetKeyCount(), 2);
	list.InsertKey(10);
	list.RemoveKey(0);
	EXPECT_EQ(list.GetKeyCount(), 2);
	list.RemoveKey(4);
	EXPECT_EQ(list.GetKeyCount(), 1);
	list.RemoveKey(10);
	EXPECT_EQ(list.GetKeyCount(), 0);
}

void SetSimpleData(ty::KeyFrameList& list) {
	list.InsertKey(2, 0.f, ty::Easings::Type::Linear);
	list.InsertKey(5, 5.f, ty::Easings::Type::Exponential);
	list.InsertKey(10, 10.f, ty::Easings::Type::Smoothstep);
}
void CheckSimpleData(ty::KeyFrameList& list) {
	{
		const ty::KeyFramePair p = list.GetKeyPair(0);
		EXPECT_EQ(p.mStart.mTick, 0);
		EXPECT_EQ(p.mEnd.mTick, 2);
		EXPECT_EQ(p.mStart.mEasingType, ty::Easings::Type::Zero);
	}
	{
		const ty::KeyFramePair p = list.GetKeyPair(1);
		EXPECT_EQ(p.mStart.mTick, 0);
		EXPECT_EQ(p.mEnd.mTick, 2);
		EXPECT_EQ(p.mStart.mEasingType, ty::Easings::Type::Zero);
	}
	{
		const ty::KeyFramePair p = list.GetKeyPair(2);
		EXPECT_EQ(p.mStart.mTick, 2);
		EXPECT_EQ(p.mEnd.mTick, 5);
		EXPECT_EQ(p.mStart.mEasingType, ty::Easings::Type::Linear);
	}
	{
		const ty::KeyFramePair p = list.GetKeyPair(3);
		EXPECT_EQ(p.mStart.mTick, 2);
		EXPECT_EQ(p.mEnd.mTick, 5);
		EXPECT_EQ(p.mStart.mEasingType, ty::Easings::Type::Linear);
	}
	{
		const ty::KeyFramePair p = list.GetKeyPair(5);
		EXPECT_EQ(p.mStart.mTick, 5);
		EXPECT_EQ(p.mEnd.mTick, 10);
		EXPECT_EQ(p.mStart.mEasingType, ty::Easings::Type::Exponential);
	}
	{
		const ty::KeyFramePair p = list.GetKeyPair(7);
		EXPECT_EQ(p.mStart.mTick, 5);
		EXPECT_EQ(p.mEnd.mTick, 10);
		EXPECT_EQ(p.mStart.mEasingType, ty::Easings::Type::Exponential);
	}
	{
		const ty::KeyFramePair p = list.GetKeyPair(10);
		EXPECT_EQ(p.mStart.mTick, 10);
		EXPECT_EQ(p.mEnd.mTick, 10);
		EXPECT_EQ(p.mStart.mEasingType, ty::Easings::Type::Smoothstep);
	}
	{
		const ty::KeyFramePair p = list.GetKeyPair(17);
		EXPECT_EQ(p.mStart.mTick, 10);
		EXPECT_EQ(p.mEnd.mTick, 10);
		EXPECT_EQ(p.mStart.mEasingType, ty::Easings::Type::Smoothstep);
	}
}
TEST(KeyFrameList, GetKeyPair) {
	ty::KeyFrameList list;

	{
		const ty::KeyFramePair p = list.GetKeyPair(2);
		EXPECT_EQ(p.mStart.mTick, 0);
		EXPECT_EQ(p.mEnd.mTick, 0);
		EXPECT_EQ(p.mStart.mEasingType, ty::Easings::Type::Zero);
	}

	SetSimpleData(list);
	CheckSimpleData(list);
}

TEST(KeyFrameList, SaveToFile) {
	{
		dubu::FileBuffer file("frames.dat", dubu::FileBuffer::Mode::Write);
		ty::KeyFrameList list;
		SetSimpleData(list);
		file << list;
	}
	{
		dubu::FileBuffer file("frames.dat", dubu::FileBuffer::Mode::Read);
		ty::KeyFrameList list;
		file >> list;
		CheckSimpleData(list);
	}
}