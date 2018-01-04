#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/ArrayStorage.hpp"
#include "caramel-math/ScalarTraits.hpp"
#include "MockErrorHandler.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;
using namespace caramel_math::matrix::test;

TEST_F(MockErrorHandlerFixtureTest, GetAndSetReturnAndUpdateStoredValue) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, MockErrorHandlerProxy>();
	storage.set(0, 0, 42.0f);
	storage.set(0, 1, 666.0f);

	EXPECT_EQ(storage.get(0, 0), 42.0f);
	EXPECT_EQ(storage.get(0, 1), 666.0f);
}

TEST_F(MockErrorHandlerFixtureTest, GetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, MockErrorHandlerProxy>();
	
	const auto errorValue = -42.0f;

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

TEST(ArrayStorageTest, GetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, NoexceptErrorHandler>();
	static_assert(noexcept(storage.get(0, 0)));
}

TEST(ArrayStorageTest, GetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.get(0, 0)));
}

TEST_F(MockErrorHandlerFixtureTest, SetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, MockErrorHandlerProxy>();

	const auto errorValue = -42.0f;

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(1, 0)).WillOnce(testing::Return(errorValue));
		storage.set(1, 0, 0.0f);
	}

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(0, 2)).WillOnce(testing::Return(errorValue));
		storage.set(0, 2, 0.0f);
	}

}

TEST(ArrayStorageTest, SetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, NoexceptErrorHandler>();
	static_assert(noexcept(storage.set(0, 0, 0.0f)));
}

TEST(ArrayStorageTest, SetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = ArrayStorage<BasicScalarTraits<float>, 1, 2, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.set(0, 0, 0.0f)));
}
