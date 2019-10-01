#include <gtest/gtest.h>

#include <takoyaki/editor/systems/animator/Easings.h>

TEST(Easings, EasingFunctions) {
	{
		float v = ty::Easings::Ease(0.f, 10.f, 0.5f, ty::Easings::Zero);
		EXPECT_EQ(v, 0.f);
	}
	{
		float v = ty::Easings::Ease(0.f, 10.f, 0.5f, ty::Easings::Linear);
		EXPECT_EQ(v, 5.f);
	}
	{
		float v = ty::Easings::Ease(0.f, 10.f, 0.5f, ty::Easings::Exponential);
		EXPECT_EQ(v, 2.5f);
	}
	{
		float v = ty::Easings::Ease(0.f, 10.f, 0.5f, ty::Easings::Smoothstep);
		EXPECT_EQ(v, 5.f);
	}
	{
		float v = ty::Easings::Ease(0.f, 10.f, -1.5f, ty::Easings::Smoothstep);
		EXPECT_EQ(v, 0.f);
	}
	{
		float v = ty::Easings::Ease(0.f, 10.f, 1.5f, ty::Easings::Smoothstep);
		EXPECT_EQ(v, 10.f);
	}
}
