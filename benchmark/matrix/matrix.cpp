#include <benchmark/benchmark.h>

#include "caramel-math/matrix/AssertErrorHandler.hpp"
#include "caramel-math/matrix/ThrowingErrorHandler.hpp"
#include "caramel-math/matrix/AffineTransformStorage.hpp"
#include "caramel-math/matrix/ArrayStorage.hpp"
#include "caramel-math/matrix/SimdStorage.hpp"
#include "caramel-math/matrix/Matrix.hpp"
#include "caramel-math/ScalarTraits.hpp"

using namespace caramel_math;
using namespace caramel_math::matrix;

//template <class StorageType>
void benchmarkMatrixMultiplicationAS(benchmark::State& state) {
	using StorageType = ArrayStorage<BasicScalarTraits<float>, 4, 4, AssertErrorHandler>;
	auto lhs = Matrix<StorageType>();
	auto rhs = Matrix<StorageType>();
	for (auto _ : state) {
		benchmark::DoNotOptimize(lhs * rhs);
	}
}

void benchmarkMatrixMultiplicationSS(benchmark::State& state) {
	using StorageType = SimdStorage<BasicScalarTraits<float>, AssertErrorHandler>;
	auto lhs = Matrix<StorageType>();
	auto rhs = Matrix<StorageType>();
	for (auto _ : state) {
		benchmark::DoNotOptimize(lhs * rhs);
	}
}

//BENCHMARK_TEMPLATE(
//	benchmarkMatrixMultiplication,
//	ArrayStorage<BasicScalarTraits<float>, 4, 4, AssertErrorHandler>,
//	ArrayStorage<BasicScalarTraits<float>, 4, 4, ThrowingErrorHandler>,
//	SimdStorage<BasicScalarTraits<float>, AssertErrorHandler>,
//	SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>
//	);

BENCHMARK(benchmarkMatrixMultiplicationAS);
BENCHMARK(benchmarkMatrixMultiplicationSS);
