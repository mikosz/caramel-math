#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "caramel-math/matrix/ThrowingErrorHandler.hpp"
#include "caramel-math/matrix/SimdStorage.hpp"
#include "caramel-math/ScalarTraits.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;

namespace /* anonymous */ {

TEST(SimdStorageTest, IsDefaultConstructible) {
	auto storage = SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>();
}

TEST(SimdStorageTest, IsConstructibleWithListOfValues) {
	using Storage = SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>;
	auto storage = Storage(
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f,
		12.0f, 13.0f, 14.0f, 15.0f
	);

	EXPECT_FLOAT_EQ(storage.get(0, 0), 0.0f);
	EXPECT_FLOAT_EQ(storage.get(0, 1), 1.0f);
	EXPECT_FLOAT_EQ(storage.get(0, 2), 2.0f);
	EXPECT_FLOAT_EQ(storage.get(0, 3), 3.0f);
	EXPECT_FLOAT_EQ(storage.get(1, 0), 4.0f);
	EXPECT_FLOAT_EQ(storage.get(1, 1), 5.0f);
	EXPECT_FLOAT_EQ(storage.get(1, 2), 6.0f);
	EXPECT_FLOAT_EQ(storage.get(1, 3), 7.0f);
	EXPECT_FLOAT_EQ(storage.get(2, 0), 8.0f);
	EXPECT_FLOAT_EQ(storage.get(2, 1), 9.0f);
	EXPECT_FLOAT_EQ(storage.get(2, 2), 10.0f);
	EXPECT_FLOAT_EQ(storage.get(2, 3), 11.0f);
	EXPECT_FLOAT_EQ(storage.get(3, 0), 12.0f);
	EXPECT_FLOAT_EQ(storage.get(3, 1), 13.0f);
	EXPECT_FLOAT_EQ(storage.get(3, 2), 14.0f);
	EXPECT_FLOAT_EQ(storage.get(3, 3), 15.0f);
}


} // anonymous namespace
