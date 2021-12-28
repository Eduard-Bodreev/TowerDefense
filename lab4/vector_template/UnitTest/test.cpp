#include "pch.h"
#include "../mvector.h"


TEST(MVectorConstructor, AllConstructors) {
	mvector<int> vec;
	EXPECT_EQ(vec.size(), 0);

	mvector<int> vec2(3);
	EXPECT_EQ(vec2.size(), 0);

	vec.push_back(1);
	vec.push_back(2);
	mvector<int> vec3(vec);
	EXPECT_EQ(vec3.size(), 2);
	EXPECT_EQ(vec3[1], 2);
}

TEST(Methods, Methods) {
	mvector<int> vec(2);
	vec.push_back(1);
	EXPECT_EQ(vec[0], 1);
	EXPECT_EQ(vec[1], 0);
	vec.push_back(2);
	vec.push_back(3);
	EXPECT_EQ(vec.size(), 3);
	EXPECT_EQ(vec[2], 3);

	vec.clear();
	EXPECT_TRUE(vec.empty());
}

TEST(Operators, Operators) {
	mvector<int> vec({ 1, 2, 3, 4 });
	mvector<int> copy = vec;
	EXPECT_TRUE(vec == copy);
	copy.clear();
	EXPECT_TRUE(vec != copy);
}

TEST(Iterator, Iterator) {
	mvector<int> vec({ 1, 2, 3, 4 });
	mvector<int>::Iterator begin = vec.begin();
	mvector<int>::Iterator end = vec.end();

	EXPECT_EQ(*begin, 1);
	begin++;
	EXPECT_EQ(*begin, 2);
	--begin;
	EXPECT_TRUE(begin == vec.begin());
	end--;
	EXPECT_EQ(*end, 4);
}

TEST(ConstIterator, ConstIterator) {
	mvector<int> vec({ 1, 2, 3, 4 });
	mvector<int>::ConstIterator begin = vec.cbegin();
	mvector<int>::ConstIterator end = vec.cend();

	EXPECT_EQ(*begin, 1);
	begin++;
	EXPECT_EQ(*begin, 2);
	--begin;
	EXPECT_TRUE(begin == vec.cbegin());
	end--;
	EXPECT_EQ(*end, 4);
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}