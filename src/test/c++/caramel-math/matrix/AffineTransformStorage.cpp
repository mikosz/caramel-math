#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/AffineTransformStorage.hpp"
#include "caramel-math/ScalarTraits.hpp"
#include "MockErrorHandler.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;
using namespace caramel_math::matrix::test;

TEST_F(MockErrorHandlerFixtureTest, GetAndSetReturnAndUpdateStoredValue) {
	auto storage = AffineTransformStorage<BasicScalarTraits<int>, MockErrorHandlerProxy>();

	storage.set(0, 0, 0);
	storage.set(0, 1, 1);
	storage.set(0, 2, 2);
	storage.set(0, 3, 3);
	storage.set(1, 0, 4);
	storage.set(1, 1, 5);
	storage.set(1, 2, 6);
	storage.set(1, 3, 7);
	storage.set(2, 0, 8);
	storage.set(2, 1, 9);
	storage.set(2, 2, 10);
	storage.set(2, 3, 11);

	EXPECT_EQ(storage.get(0, 0), 0);
	EXPECT_EQ(storage.get(0, 1), 1);
	EXPECT_EQ(storage.get(0, 2), 2);
	EXPECT_EQ(storage.get(0, 3), 3);
	EXPECT_EQ(storage.get(1, 0), 4);
	EXPECT_EQ(storage.get(1, 1), 5);
	EXPECT_EQ(storage.get(1, 2), 6);
	EXPECT_EQ(storage.get(1, 3), 7);
	EXPECT_EQ(storage.get(2, 0), 8);
	EXPECT_EQ(storage.get(2, 1), 9);
	EXPECT_EQ(storage.get(2, 2), 10);
	EXPECT_EQ(storage.get(2, 3), 11);

	EXPECT_EQ(storage.get(3, 0), 0);
	EXPECT_EQ(storage.get(3, 1), 0);
	EXPECT_EQ(storage.get(3, 2), 0);
	EXPECT_EQ(storage.get(3, 3), 1);
}

TEST_F(MockErrorHandlerFixtureTest, SetCallsInvalidValueForValuesNotMatchingAnAffineTransformMatrix) {
	auto storage = AffineTransformStorage<BasicScalarTraits<int>, MockErrorHandlerProxy>();

	{
		testing::InSequence();
		EXPECT_CALL(*MockErrorHandler::instance, invalidValue(3, 0, 1, 0));
		EXPECT_CALL(*MockErrorHandler::instance, invalidValue(3, 1, 1, 0));
		EXPECT_CALL(*MockErrorHandler::instance, invalidValue(3, 2, 1, 0));
		EXPECT_CALL(*MockErrorHandler::instance, invalidValue(3, 3, 0, 1));
	}

	storage.set(3, 0, 1);
	storage.set(3, 1, 1);
	storage.set(3, 2, 1);
	storage.set(3, 3, 0);
}
