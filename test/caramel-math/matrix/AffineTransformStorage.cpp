#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/AffineTransformStorage.hpp"
#include "caramel-math/scalar/ScalarTraits.hpp"
#include "MockErrorHandler.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;
using namespace caramel_math::scalar;
using namespace caramel_math::matrix::literals;
using namespace caramel_math::matrix::test;

namespace /* anonymous */ {

class AffineTransformStorageTest : public MockErrorHandlerFixtureTest {
};

TEST_F(AffineTransformStorageTest, IsDefaultConstructible) {
	auto storage = AffineTransformStorage<BasicScalarTraits<int>, MockErrorHandlerProxy>();
}

TEST_F(AffineTransformStorageTest, IsConstructibleWithListOfValues) {
	using Storage = AffineTransformStorage<BasicScalarTraits<int>, MockErrorHandlerProxy>;
	auto storage = Storage(
		0, 1, 2, 3,
		4, 5, 6, 7,
		8, 9, 10, 11
		);

	EXPECT_EQ(storage.get(0_row, 0_col), 0);
	EXPECT_EQ(storage.get(0_row, 1_col), 1);
	EXPECT_EQ(storage.get(0_row, 2_col), 2);
	EXPECT_EQ(storage.get(0_row, 3_col), 3);
	EXPECT_EQ(storage.get(1_row, 0_col), 4);
	EXPECT_EQ(storage.get(1_row, 1_col), 5);
	EXPECT_EQ(storage.get(1_row, 2_col), 6);
	EXPECT_EQ(storage.get(1_row, 3_col), 7);
	EXPECT_EQ(storage.get(2_row, 0_col), 8);
	EXPECT_EQ(storage.get(2_row, 1_col), 9);
	EXPECT_EQ(storage.get(2_row, 2_col), 10);
	EXPECT_EQ(storage.get(2_row, 3_col), 11);
}

TEST_F(AffineTransformStorageTest, GetAndSetReturnAndUpdateStoredValue) {
	auto storage = AffineTransformStorage<BasicScalarTraits<int>, MockErrorHandlerProxy>();

	storage.set(0_row, 0_col, 0);
	storage.set(0_row, 1_col, 1);
	storage.set(0_row, 2_col, 2);
	storage.set(0_row, 3_col, 3);
	storage.set(1_row, 0_col, 4);
	storage.set(1_row, 1_col, 5);
	storage.set(1_row, 2_col, 6);
	storage.set(1_row, 3_col, 7);
	storage.set(2_row, 0_col, 8);
	storage.set(2_row, 1_col, 9);
	storage.set(2_row, 2_col, 10);
	storage.set(2_row, 3_col, 11);

	EXPECT_EQ(storage.get(0_row, 0_col), 0);
	EXPECT_EQ(storage.get(0_row, 1_col), 1);
	EXPECT_EQ(storage.get(0_row, 2_col), 2);
	EXPECT_EQ(storage.get(0_row, 3_col), 3);
	EXPECT_EQ(storage.get(1_row, 0_col), 4);
	EXPECT_EQ(storage.get(1_row, 1_col), 5);
	EXPECT_EQ(storage.get(1_row, 2_col), 6);
	EXPECT_EQ(storage.get(1_row, 3_col), 7);
	EXPECT_EQ(storage.get(2_row, 0_col), 8);
	EXPECT_EQ(storage.get(2_row, 1_col), 9);
	EXPECT_EQ(storage.get(2_row, 2_col), 10);
	EXPECT_EQ(storage.get(2_row, 3_col), 11);
	
	EXPECT_EQ(storage.get(3_row, 0_col), 0);
	EXPECT_EQ(storage.get(3_row, 1_col), 0);
	EXPECT_EQ(storage.get(3_row, 2_col), 0);
	EXPECT_EQ(storage.get(3_row, 3_col), 1);
}

TEST_F(AffineTransformStorageTest, SetCallsInvalidValueForValuesNotMatchingAnAffineTransformMatrix) {
	auto storage = AffineTransformStorage<BasicScalarTraits<int>, MockErrorHandlerProxy>();

	{
		testing::InSequence();
		EXPECT_CALL(*MockErrorHandler::instance, invalidValue(3_row, 0_col, 1, 0));
		EXPECT_CALL(*MockErrorHandler::instance, invalidValue(3_row, 1_col, 1, 0));
		EXPECT_CALL(*MockErrorHandler::instance, invalidValue(3_row, 2_col, 1, 0));
		EXPECT_CALL(*MockErrorHandler::instance, invalidValue(3_row, 3_col, 0, 1));
	}

	storage.set(3_row, 0_col, 1);
	storage.set(3_row, 1_col, 1);
	storage.set(3_row, 2_col, 1);
	storage.set(3_row, 3_col, 0);
}

TEST_F(AffineTransformStorageTest, GetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = AffineTransformStorage<BasicScalarTraits<int>, MockErrorHandlerProxy>();

	const auto errorValue = -42;

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(4_row, 0_col)).WillOnce(testing::Return(errorValue));
		const auto value = storage.get(4_row, 0_col);
		EXPECT_EQ(errorValue, value);
	}

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(0_row, 4_col)).WillOnce(testing::Return(errorValue));
		const auto value = storage.get(0_row, 4_col);
		EXPECT_EQ(errorValue, value);
	}

}

TEST_F(AffineTransformStorageTest, GetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = AffineTransformStorage<BasicScalarTraits<float>, NoexceptErrorHandler>();
	static_assert(noexcept(storage.get(0_row, 0_col)));
}

TEST_F(AffineTransformStorageTest, GetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = AffineTransformStorage<BasicScalarTraits<float>, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.get(0_row, 0_col)));
}

TEST_F(AffineTransformStorageTest, SetWithOutOfBoundsIndexCallsErrorHandler) {
	static_assert(RUNTIME_CHECKS);

	auto storage = AffineTransformStorage<BasicScalarTraits<int>, MockErrorHandlerProxy>();

	const auto errorValue = -42;

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(4_row, 0_col)).WillOnce(testing::Return(errorValue));
		storage.set(4_row, 0_col, 0);
	}

	{
		EXPECT_CALL(*MockErrorHandler::instance, invalidAccess(0_row, 4_col)).WillOnce(testing::Return(errorValue));
		storage.set(0_row, 4_col, 0);
	}

}

TEST_F(AffineTransformStorageTest, SetIsNoexceptIfErrorHandlerInvalidAccessIsNoexcept) {
	auto storage = AffineTransformStorage<BasicScalarTraits<float>, NoexceptErrorHandler>();
	static_assert(noexcept(storage.set(0_row, 0_col, 0.0f)));
}

TEST_F(AffineTransformStorageTest, SetIsPotentiallyThrowingIfErrorHandlerInvalidAccessIsPotentiallyThrowing) {
	auto storage = AffineTransformStorage<BasicScalarTraits<float>, PotentiallyThrowingErrorHandler>();
	static_assert(!noexcept(storage.set(0_row, 0_col, 0.0f)));
}

} // anonymous namespace
