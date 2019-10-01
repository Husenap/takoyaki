#include <gtest/gtest.h>

#include <numeric>

#include "serializer/Serializer.h"

void WriteSimpleData(dubu::WriteBuffer& buffer) {
	buffer << 12 << 24 << 36 << 48.f << 64.0;
}
void ReadSimpleData(dubu::ReadBuffer& buffer) {
	int numbers[3];
	float f;
	double d;
	buffer >> numbers[0] >> numbers[1] >> numbers[2] >> f >> d;
	EXPECT_EQ(numbers[0], 12);
	EXPECT_EQ(numbers[1], 24);
	EXPECT_EQ(numbers[2], 36);
	EXPECT_EQ(f, 48.f);
	EXPECT_EQ(d, 64.0);
}

const char* LongStringText =
    "Having a long string in a binary file and just dumping its contents can be very dangerous";
void WriteComplexData(dubu::WriteBuffer& buffer) {
	buffer << std::string(LongStringText) << 3.14159265 << 1337;
}
void ReadComplexData(dubu::ReadBuffer& buffer) {
	std::string text;
	double pi;
	int leet;
	buffer >> text >> pi >> leet;
	EXPECT_EQ(text, LongStringText);
	EXPECT_EQ(pi, 3.14159265);
	EXPECT_EQ(leet, 1337);
}

TEST(Serializer, FileSimpleData) {
	{
		dubu::FileBuffer fileBuffer("FileSimpleData.bin", dubu::FileBuffer::Mode::Write);
		WriteSimpleData(fileBuffer);
	}
	{
		dubu::FileBuffer fileBuffer("FileSimpleData.bin", dubu::FileBuffer::Mode::Read);
		ReadSimpleData(fileBuffer);
	}
}

TEST(Serializer, FileComplexData) {
	{
		dubu::FileBuffer fileBuffer("FileComplexData.bin", dubu::FileBuffer::Mode::Write);
		WriteComplexData(fileBuffer);
	}
	{
		dubu::FileBuffer fileBuffer("FileComplexData.bin", dubu::FileBuffer::Mode::Read);
		ReadComplexData(fileBuffer);
	}
}

TEST(Serializer, Vector) {
	std::vector<int> ints(19);
	std::vector<float> floats(19);
	std::vector<double> doubles(19);
	std::vector<std::string> strings{"hello", "there", "my", "friend", "!"};
	std::iota(ints.begin(), ints.end(), -9);
	std::iota(floats.begin(), floats.end(), -9.f);
	std::iota(doubles.begin(), doubles.end(), -9.0);
	{
		dubu::FileBuffer fileBuffer("Vector.bin", dubu::FileBuffer::Mode::Write);
		fileBuffer << ints << floats << doubles << strings;
	}

	std::vector<int> inInts;
	std::vector<float> inFloats;
	std::vector<double> inDoubles;
	std::vector<std::string> inStrings;
	{
		dubu::FileBuffer fileBuffer("Vector.bin", dubu::FileBuffer::Mode::Read);
		fileBuffer >> inInts >> inFloats >> inDoubles >> inStrings;
	}

	EXPECT_EQ(ints, inInts);
	EXPECT_EQ(floats, inFloats);
	EXPECT_EQ(doubles, inDoubles);
	EXPECT_EQ(strings, inStrings);
}
