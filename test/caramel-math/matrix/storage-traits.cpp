#include <gtest/gtest.h>

#include "caramel-math/matrix/storage-traits.hpp"

#include "caramel-math/matrix/Matrix.hpp"
#include "caramel-math/matrix/ViewStorage.hpp"
#include "caramel-math/matrix/ArrayStorage.hpp"
#include "caramel-math/matrix/AssertErrorHandler.hpp"
#include "caramel-math/scalar/ScalarTraits.hpp"

using namespace caramel_math;
using namespace caramel_math::scalar;
using namespace caramel_math::matrix;

namespace /* anonymous */ {

struct NullStorage {
	using Scalar = float;
	static constexpr auto ROWS = 0;
	static constexpr auto COLUMNS = 0;
};

TEST(StorageTraitsTest, EffectiveStorageTypeIsStorageType) {
	using ArrayStorage = ArrayStorage<BasicScalarTraits<float>, 1, 2, AssertErrorHandler>;
	static_assert(std::is_same_v<EffectiveStorageType<ArrayStorage>, ArrayStorage>);
}

TEST(StorageTraitsTest, EffectiveStorageTypeOfViewStorageIsViewedStorageType) {
	using ArrayStorage = ArrayStorage<BasicScalarTraits<float>, 1, 2, AssertErrorHandler>;
	using ViewStorage = TransposedViewStorage<Matrix<ArrayStorage>>;
	static_assert(std::is_same_v<EffectiveStorageType<ViewStorage>, ArrayStorage>);
}

TEST(StorageTraitsTest, EffectiveStorageTypeOfNestedViewStorageIsViewedStorageType) {
	using ArrayStorage = ArrayStorage<BasicScalarTraits<float>, 1, 2, AssertErrorHandler>;
	using ViewStorage = TransposedViewStorage<Matrix<ArrayStorage>>;
	using NestedViewStorage = TransposedViewStorage<Matrix<ViewStorage>>;
	static_assert(std::is_same_v<EffectiveStorageType<NestedViewStorage>, ArrayStorage>);
}

TEST(StorageTraitsTest, BinaryOperatorResultTypeWithLHSArrayStorageIsArrayStorage) {
	using ArrayStorage = ArrayStorage<BasicScalarTraits<float>, 1, 2, AssertErrorHandler>;
	static_assert(std::is_same_v<BinaryOperatorResultType<ArrayStorage, NullStorage, 1, 2>, ArrayStorage>);
}

TEST(StorageTraitsTest, BinaryOperatorResultTypeWithRHSArrayStorageIsArrayStorage) {
	using ArrayStorage = ArrayStorage<BasicScalarTraits<float>, 1, 2, AssertErrorHandler>;
	static_assert(std::is_same_v<BinaryOperatorResultType<NullStorage, ArrayStorage, 1, 2>, ArrayStorage>);
}

TEST(StorageTraitsTest, BinaryOperatorResultTypeWithBothArrayStorageIsLHSArrayStorage) {
	using LHSArrayStorage = ArrayStorage<BasicScalarTraits<float>, 1, 2, AssertErrorHandler>;
	using RHSArrayStorage = ArrayStorage<BasicScalarTraits<float>, 3, 4, AssertErrorHandler>;
	static_assert(std::is_same_v<BinaryOperatorResultType<LHSArrayStorage, RHSArrayStorage, 1, 2>, LHSArrayStorage>);
}

TEST(StorageTraitsTest, BinaryOperatorResultTypeWithSameStorageTypesIsTheStorageType) {
	using ArrayStorage = ArrayStorage<BasicScalarTraits<float>, 1, 2, AssertErrorHandler>;
	static_assert(std::is_same_v<BinaryOperatorResultType<ArrayStorage, ArrayStorage, 1, 2>, ArrayStorage>);

	static_assert(std::is_same_v<BinaryOperatorResultType<NullStorage, NullStorage, 0, 0>, NullStorage>);
}

} // anonymous namespace
