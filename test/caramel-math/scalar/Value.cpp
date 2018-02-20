#include <gtest/gtest.h>

#include "caramel-math/scalar/Value.hpp"

using namespace caramel_math::scalar;

namespace /* anonymous */ {

class SomeValue : Value<SomeValue, int> {
public:

	constexpr SomeValue() = default;

	constexpr explicit SomeValue(int value) :
		Value<SomeValue, int>(value)
	{
	}

	constexpr int value() const noexcept {
		return Value<SomeValue, int>::value();
	}

};

TEST(ValueTest, ValuesAreZeroByDefault) {
	constexpr auto zero = SomeValue();
	static_assert(zero.value() == 0);
}

TEST(ValueTest, ValuesAreAssignable) {
	const auto three = SomeValue(3);
	auto assigned = SomeValue();
	assigned = three;

	EXPECT_EQ(assigned.value(), three.value());
}

TEST(ValueTest, ValuesAreComparable) {
	constexpr auto three = SomeValue(3);
	constexpr auto otherThree = SomeValue(3);
	constexpr auto four = SomeValue(4);

	static_assert(three == otherThree);
	static_assert(three != four);
	static_assert(three <= otherThree);
	static_assert(three < four);
	static_assert(three >= otherThree);
	static_assert(four > three);
}

TEST(ValueTest, ValuesAreAdditive) {
	constexpr auto three = SomeValue(3);
	constexpr auto four = SomeValue(4);
	constexpr auto seven = SomeValue(7);

	auto zeroPlusEqThree = SomeValue(0);
	zeroPlusEqThree += three;

	auto sevenMinusEqFour = seven;
	sevenMinusEqFour -= four;

	static_assert(three + four == seven);
	static_assert(seven - four == three);
	EXPECT_EQ(zeroPlusEqThree, three);
	EXPECT_EQ(sevenMinusEqFour, three);
}

TEST(ValueTest, ValuesAreIncrementableAndDecrementable) {
	auto oneTwoThree = SomeValue(1);

	EXPECT_EQ(++oneTwoThree, SomeValue(2));
	EXPECT_EQ(oneTwoThree++, SomeValue(2));
	EXPECT_EQ(oneTwoThree, SomeValue(3));

	EXPECT_EQ(--oneTwoThree, SomeValue(2));
	EXPECT_EQ(oneTwoThree--, SomeValue(2));
	EXPECT_EQ(oneTwoThree, SomeValue(1));
}

TEST(ValueTest, ValuesAreMultiplicative) {
	constexpr auto three = SomeValue(3);
	constexpr auto four = SomeValue(4);
	constexpr auto twelve = SomeValue(12);

	auto threeTimesEqFour = three;
	threeTimesEqFour *= 4;

	auto twelveDivEqFour = twelve;
	twelveDivEqFour /= 4;

	auto threeModEqFour = three;
	threeModEqFour %= 4;

	static_assert(three * 4 == twelve);
	static_assert(twelve / 4 == three);
	static_assert(three % 4 == three);
	EXPECT_EQ(threeTimesEqFour, twelve);
	EXPECT_EQ(twelveDivEqFour, three);
	EXPECT_EQ(threeModEqFour, three);
}

} // anonymous namespace
