#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/ArrayStorage.hpp"
#include "caramel-math/scalar/ScalarTraits.hpp"
#include "MockErrorHandler.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;
using namespace caramel_math::scalar;
using namespace caramel_math::matrix::literals;
using namespace caramel_math::matrix::test;

namespace /* anonymous */ {

class ArrayStorageTest : public MockErrorHandlerFixtureTest {
};

TEST_F(ArrayStorageTest, IsDefaultConstructible) {
	auto storage = ArrayStorage<BasicScalarTraits<int>, 1, 2, MockErrorHandlerProxy>();
}

TEST_F(ArrayStorageTest, IsConstructibleWithListOfValues) {
	using Storage = ArrayStorage<BasicScalarTraits<int>, 2, 3, MockErrorHandlerProxy>;
	auto storage = Storage(
		0, 1, 2,
		3, 4, 5
		);

	EXPECT_EQ(storage.get(0_row, 0_col), 0);
	EXPECT_EQ(storage.get(0_row, 1_col), 1);
	EXPECT_EQ(storage.get(0_row, 2_col), 2);
	EXPECT_EQ(storage.get(1_row, 0_col), 3);
	EXPECT_EQ(storage.get(1_row, 1_col), 4);
	EXPECT_EQ(storage.get(1_row, 2_col), 5);
}

TEST_F(ArrayStorageTest, ArrayStorageIsCopyable) {
	using Storage = ArrayStorage<BasicScalarTraits<int>, 2, 3, MockErrorHandlerProxy>;
	auto storage = Storage(
		0, 1, 2,
		3, 4, 5
		);
	const auto copy = storage;

	EXPECT_EQ(copy.get(0_row, 0_col), 0);
	EXPECT_EQ(copy.get(0_row, 1_col), 1);
	EXPECT_EQ(copy.get(0_row, 2_col), 2);
	EXPECT_EQ(copy.get(1_row, 0_col), 3);
	EXPECT_EQ(copy.get(1_row, 1_col), 4);
	EXPECT_EQ(copy.get(1_row, 2_col), 5);
}

TEST_F(ArrayStorageTest, GetAndSetReturnAndUpdateStoredValue) {
	auto storage = ArrayStorage<BasicScalarTraits<int>, 1, 2, MockErrorHandlerProxy>();
	storage.set(0_row, 0_col, 42);
	storage.set(0_row, 1_col, 666);

	EXPECT_EQ(storage.get(0_row, 0_col), 42);
	EXPECT_EQ(storage.get(0_row, 1_col), 666);
}

TEST_F(ArrayStorageTest, GetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = ArrayStorage<BasicScalarTraits<int>, 1, 2, MockErrorHandlerProxy>();
	
	const auto errorValue = -42;

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(1_row, 0_col)).WillOnce(testing::Return(errorValue));
		const auto value = storage.get(1_row, 0_col);
		EXPECT_EQ(errorValue, value);
	}

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(0_row, 2_col)).WillOnce(testing::Return(errorValue));
		const auto value = storage.get(0_row, 2_col);
		EXPECT_EQ(errorValue, value);
	}

}

TEST_F(ArrayStorageTest, GetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, NoexceptErrorHandler>();
	static_assert(noexcept(storage.get(0_row, 0_col)));
}

TEST_F(ArrayStorageTest, GetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.get(0_row, 0_col)));
}

TEST_F(ArrayStorageTest, SetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = ArrayStorage<BasicScalarTraits<int>, 1, 2, MockErrorHandlerProxy>();

	const auto errorValue = -42;

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(1_row, 0_col)).WillOnce(testing::Return(errorValue));
		storage.set(1_row, 0_col, 0);
	}

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(0_row, 2_col)).WillOnce(testing::Return(errorValue));
		storage.set(0_row, 2_col, 0);
	}

}

TEST_F(ArrayStorageTest, SetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, NoexceptErrorHandler>();
	static_assert(noexcept(storage.set(0_row, 0_col, 0.0f)));
}

TEST_F(ArrayStorageTest, SetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.set(0_row, 0_col, 0.0f)));
}

} // anonymous namespace
