#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/ArrayStorage.hpp"
#include "caramel-math/ScalarTraits.hpp"
#include "MockErrorHandler.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;
using namespace caramel_math::matrix::test;

namespace /* anonymous */ {

class ArrayStorageTest : public MockErrorHandlerFixtureTest {
};

TEST_F(ArrayStorageTest, ArrayStorageIsDefaultConstructible) {
	auto storage = ArrayStorage<BasicScalarTraits<int>, 1, 2, MockErrorHandlerProxy>();
}

TEST_F(ArrayStorageTest, ArrayStorageIsConstructibleWithListOfValues) {
	using Storage = ArrayStorage<BasicScalarTraits<int>, 2, 3, MockErrorHandlerProxy>;
	auto storage = Storage(
		0, 1, 2,
		3, 4, 5
		);

	EXPECT_EQ(storage.get(0, 0), 0);
	EXPECT_EQ(storage.get(0, 1), 1);
	EXPECT_EQ(storage.get(0, 2), 2);
	EXPECT_EQ(storage.get(1, 0), 3);
	EXPECT_EQ(storage.get(1, 1), 4);
	EXPECT_EQ(storage.get(1, 2), 5);
}

TEST_F(ArrayStorageTest, ArrayStorageIsCopyable) {
	using Storage = ArrayStorage<BasicScalarTraits<int>, 2, 3, MockErrorHandlerProxy>;
	auto storage = Storage(
		0, 1, 2,
		3, 4, 5
		);
	const auto copy = storage;

	EXPECT_EQ(copy.get(0, 0), 0);
	EXPECT_EQ(copy.get(0, 1), 1);
	EXPECT_EQ(copy.get(0, 2), 2);
	EXPECT_EQ(copy.get(1, 0), 3);
	EXPECT_EQ(copy.get(1, 1), 4);
	EXPECT_EQ(copy.get(1, 2), 5);
}

TEST_F(ArrayStorageTest, GetAndSetReturnAndUpdateStoredValue) {
	auto storage = ArrayStorage<BasicScalarTraits<int>, 1, 2, MockErrorHandlerProxy>();
	storage.set(0, 0, 42);
	storage.set(0, 1, 666);

	EXPECT_EQ(storage.get(0, 0), 42);
	EXPECT_EQ(storage.get(0, 1), 666);
}

TEST_F(ArrayStorageTest, GetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = ArrayStorage<BasicScalarTraits<int>, 1, 2, MockErrorHandlerProxy>();
	
	const auto errorValue = -42;

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(1, 0)).WillOnce(testing::Return(errorValue));
		const auto value = storage.get(1, 0);
		EXPECT_EQ(errorValue, value);
	}

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(0, 2)).WillOnce(testing::Return(errorValue));
		const auto value = storage.get(0, 2);
		EXPECT_EQ(errorValue, value);
	}

}

TEST_F(ArrayStorageTest, GetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, NoexceptErrorHandler>();
	static_assert(noexcept(storage.get(0, 0)));
}

TEST_F(ArrayStorageTest, GetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.get(0, 0)));
}

TEST_F(ArrayStorageTest, SetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = ArrayStorage<BasicScalarTraits<int>, 1, 2, MockErrorHandlerProxy>();

	const auto errorValue = -42;

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(1, 0)).WillOnce(testing::Return(errorValue));
		storage.set(1, 0, 0);
	}

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(0, 2)).WillOnce(testing::Return(errorValue));
		storage.set(0, 2, 0);
	}

}

TEST_F(ArrayStorageTest, SetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, NoexceptErrorHandler>();
	static_assert(noexcept(storage.set(0, 0, 0.0f)));
}

TEST_F(ArrayStorageTest, SetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.set(0, 0, 0.0f)));
}

} // anonymous namespace
