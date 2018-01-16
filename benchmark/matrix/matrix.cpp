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

namespace /* anonymous */ {

using ArrayNoexcept = ArrayStorage<BasicScalarTraits<float>, 4, 4, AssertErrorHandler>;
using ArrayThrowing = ArrayStorage<BasicScalarTraits<float>, 4, 4, ThrowingErrorHandler>;
using SimdNoexcept = SimdStorage<BasicScalarTraits<float>, AssertErrorHandler>;
using SimdThrowing = SimdStorage<BasicScalarTraits<float>, ThrowingErrorHandler>;

template <class StorageType>
void benchmarkMatrixMultiplication(benchmark::State& state) {
	auto lhs = Matrix<StorageType>();
	auto rhs = Matrix<StorageType>();
	for (auto _ : state) {
		benchmark::DoNotOptimize(lhs * rhs);
	}
}

BENCHMARK_TEMPLATE(benchmarkMatrixMultiplication, ArrayNoexcept);
BENCHMARK_TEMPLATE(benchmarkMatrixMultiplication, ArrayThrowing);
BENCHMARK_TEMPLATE(benchmarkMatrixMultiplication, SimdNoexcept);
BENCHMARK_TEMPLATE(benchmarkMatrixMultiplication, SimdThrowing);

template <class StorageType>
void benchmarkMatrixGet(benchmark::State& state) {
	auto m = Matrix<StorageType>();
	for (auto _ : state) {
		benchmark::DoNotOptimize(m.get(1, 2));
	}
}

BENCHMARK_TEMPLATE(benchmarkMatrixGet, ArrayNoexcept);
BENCHMARK_TEMPLATE(benchmarkMatrixGet, ArrayThrowing);
BENCHMARK_TEMPLATE(benchmarkMatrixGet, SimdNoexcept);
BENCHMARK_TEMPLATE(benchmarkMatrixGet, SimdThrowing);

template <class StorageType>
void benchmarkMatrixGetColumn(benchmark::State& state) {
	auto m = Matrix<StorageType>();
	for (auto _ : state) {
		benchmark::DoNotOptimize(m.get(1));
	}
}

BENCHMARK_TEMPLATE(benchmarkMatrixGetColumn, SimdNoexcept);
BENCHMARK_TEMPLATE(benchmarkMatrixGetColumn, SimdThrowing);

template <class StorageType>
void benchmarkMatrixSetGet(benchmark::State& state) {
	auto m = Matrix<StorageType>();
	for (auto _ : state) {
		m.set(1, 2, 0.3f);
		benchmark::DoNotOptimize(m.get(1, 2));
	}
}

BENCHMARK_TEMPLATE(benchmarkMatrixSetGet, ArrayNoexcept);
BENCHMARK_TEMPLATE(benchmarkMatrixSetGet, ArrayThrowing);
BENCHMARK_TEMPLATE(benchmarkMatrixSetGet, SimdNoexcept);
BENCHMARK_TEMPLATE(benchmarkMatrixSetGet, SimdThrowing);

template <class StorageType>
void benchmarkMatrixSetGetColumn(benchmark::State& state) {
	auto m = Matrix<StorageType>();
	auto f4 = simd::Float4({ 0.0f, 1.0f, 2.0f, 3.0f });
	for (auto _ : state) {
		m.set(1, f4);
		benchmark::DoNotOptimize(m.get(1));
	}
}

BENCHMARK_TEMPLATE(benchmarkMatrixSetGetColumn, SimdNoexcept);
BENCHMARK_TEMPLATE(benchmarkMatrixSetGetColumn, SimdThrowing);

} // anonymous namespace
