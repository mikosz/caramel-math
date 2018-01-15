#ifndef CARAMELMATH_MATRIX_SIMDSTORAGE_HPP__
#define CARAMELMATH_MATRIX_SIMDSTORAGE_HPP__

#include <array>

#include "../simd/Float4.hpp"
#include "../setup.hpp"
#include "matrixfwd.hpp"

namespace caramel_math::matrix {

template <class ScalarTraitsType, class ErrorHandlerType>
class SimdStorage {
public:

	using ScalarTraits = ScalarTraitsType;

	static_assert(std::is_same_v<typename ScalarTraits::Scalar, float>);

	using Scalar = float;

	using ErrorHandler = ErrorHandlerType;

	static constexpr auto ROWS = 4;

	static constexpr auto COLUMNS = 4;

	SimdStorage() = default;

	template <class... CompatibleValues>
	explicit SimdStorage(CompatibleValues&&... values) noexcept
	{
		static_assert(sizeof...(values) == ROWS * COLUMNS);
		init_(std::forward<CompatibleValues>(values)...);
	}

	float get(size_t row, size_t column) const noexcept(
		noexcept(ErrorHandler::invalidAccess<float>(row, column)))
	{
		if constexpr (RUNTIME_CHECKS) {
			if (row >= ROWS || column >= COLUMNS) {
				return ErrorHandler::invalidAccess<float>(row, column);
			}
		}
		return columns_[column].xyzw()[row];
	}

	void set(size_t row, size_t column, Scalar scalar) noexcept(
		noexcept(ErrorHandler::invalidAccess<float>(row, column)))
	{
		if constexpr (RUNTIME_CHECKS) {
			if (row >= ROWS || column >= COLUMNS) {
				ErrorHandler::invalidAccess<float>(row, column);
				return;
			}
		}
		auto xyzw = columns_[column].xyzw();
		xyzw[row] = std::move(scalar);
		columns_[column] = Float4(xyzw);
	}

	simd::Float4 get(size_t column) const noexcept(
		noexcept(ErrorHandler::invalidAccess<simd::Float4>(0, column))) // TODO
	{
		if constexpr (RUNTIME_CHECKS) {
			if (column >= COLUMNS) {
				return ErrorHandler::invalidAccess<simd::Float4>(0, column); // TODO
			}
		}
		return columns_[column];
	}

	void set(size_t column, simd::Float4 value) noexcept(
		noexcept(ErrorHandler::invalidAccess<simd::Float4>(0, column))) // TODO
	{
		if constexpr (RUNTIME_CHECKS) {
			if (column >= COLUMNS) {
				ErrorHandler::invalidAccess<simd::Float4>(0, column); // TODO
				return;
			}
		}
		columns_[column] = std::move(value);
	}

private:

	std::array<simd::Float4, COLUMNS> columns_;

	template <class... Tail>
	void init_(Tail&&... tail) noexcept {
		const auto rawData = std::array<float, ROWS * COLUMNS>{ std::forward<Tail>(tail)... };
		for (auto columnIdx = 0u; columnIdx < COLUMNS; ++columnIdx) {
			columns_[columnIdx] = simd::Float4({
				rawData[0 * COLUMNS + columnIdx],
				rawData[1 * COLUMNS + columnIdx],
				rawData[2 * COLUMNS + columnIdx],
				rawData[3 * COLUMNS + columnIdx]
				});
		}
	}

};

template <
	class LHSScalarTraitsType,
	class LHSErrorHandlerType,
	class RHSScalarTraitsType,
	class RHSErrorHandlerType
	>
inline auto operator*(
	const Matrix<SimdStorage<LHSScalarTraitsType, LHSErrorHandlerType>>& lhs,
	const Matrix<SimdStorage<RHSScalarTraitsType, RHSErrorHandlerType>>& rhs
	) noexcept(noexcept(lhs.get(0, 0)) && noexcept(rhs.get(0, 0))) // TODO
{
	static_assert(SimdStorage<LHSScalarTraitsType, LHSErrorHandlerType>::COLUMNS == 4);

	auto result = Matrix<SimdStorage<LHSScalarTraitsType, LHSErrorHandlerType>>();

	for (auto columnIdx = 0u; columnIdx < 4; ++columnIdx) {
		const auto rhsColumnXyzw = rhs.data().get(columnIdx).xyzw();

		auto column = lhs.data().get(0) * simd::Float4(rhsColumnXyzw[0]);
		column += lhs.data().get(1) * simd::Float4(rhsColumnXyzw[1]);
		column += lhs.data().get(2) * simd::Float4(rhsColumnXyzw[2]);
		column += lhs.data().get(3) * simd::Float4(rhsColumnXyzw[3]);

		result.data().set(columnIdx, column);
	}

	return result;
}

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_SIMDSTORAGE_HPP__ */
