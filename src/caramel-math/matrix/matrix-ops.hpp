#ifndef CARAMELMATH_MATRIX_MATRIXOPS_HPP__
#define CARAMELMATH_MATRIX_MATRIXOPS_HPP__

#include "Matrix.hpp"
#include "ViewStorage.hpp"

namespace caramel_math::matrix {

template <class ResultType, class StorageType>
inline [[nodiscard]] ResultType transposed(const Matrix<StorageType>& matrix) {
	return ResultType(viewTransposed(matrix));
}

template <class ResultType, class ViewedMatrixType>
inline [[nodiscard]] ResultType transposed(const Matrix<TransposedViewStorage<ViewedMatrixType>>& transposedMatrixView) {
	return matrix.viewedMatrix();
}

template <class LHSStorageType, class RHSStorageType>
inline [[nodiscard]] bool operator==(
	const Matrix<LHSStorageType>& lhs,
	const Matrix<RHSStorageType>& rhs
	) noexcept(noexcept(lhs.get(0, 0)) && noexcept(rhs.get(0, 0)))
{
	static_assert(
		LHSStorageType::COLUMNS == RHSStorageType::COLUMNS &&
		LHSStorageType::ROWS == RHSStorageType::ROWS,
		"Incompatible matrix sizes for equality test"
		);

	for (auto rowIdx = 0u; rowIdx < LHSStorageType::ROWS; ++rowIdx) {
		for (auto columnIdx = 0u; columnIdx < LHSStorageType::COLUMNS; ++columnIdx) {
			using ScalarTraits = typename LHSStorageType::ScalarTraits;
			if (!ScalarTraits::equal(lhs.get(rowIdx, columnIdx), rhs.get(rowIdx, columnIdx))) {
				return false;
			}
		}
	}

	return true;
}

template <class LHSStorageType, class RHSStorageType>
inline [[nodiscard]] bool operator!=(
	const Matrix<LHSStorageType>& lhs,
	const Matrix<RHSStorageType>& rhs
	) noexcept(noexcept(lhs == rhs))
{
	return !(lhs == rhs);
}

template <class LHSStorageType, class RHSStorageType>
inline [[nodiscard]] auto operator*(
	const Matrix<LHSStorageType>& lhs,
	const Matrix<RHSStorageType>& rhs
	) noexcept(noexcept(lhs.get(0, 0)) && noexcept(rhs.get(0, 0)))
{
	static_assert(LHSStorageType::COLUMNS == RHSStorageType::ROWS, "Incompatible matrix sizes for multiplication");
	using ResultStorageType =
		BinaryOperatorResultType<LHSStorageType, RHSStorageType, LHSStorageType::ROWS, RHSStorageType::COLUMNS>;
	using ResultType = Matrix<ResultStorageType>;

	auto result = ResultType();

	for (auto rowIdx = 0u; rowIdx < LHSStorageType::ROWS; ++rowIdx) {
		for (auto columnIdx = 0u; columnIdx < RHSStorageType::COLUMNS; ++columnIdx) {
			auto dot = ResultType::ScalarTraits::ZERO;
			for (auto dotIdx = 0u; dotIdx < LHSStorageType::COLUMNS; ++dotIdx) {
				dot += lhs.get(rowIdx, dotIdx) * rhs.get(dotIdx, columnIdx);
			}
			result.set(rowIdx, columnIdx, dot);
		}
	}

	return result;
}

template <class LHSStorageType, class RHSStorageType>
inline auto& operator*=(
	Matrix<LHSStorageType>& lhs,
	const Matrix<RHSStorageType>& rhs
	) noexcept(noexcept(lhs * rhs))
{
	lhs = lhs * rhs;
	return lhs;
}

template <class StorageType>
inline Matrix<StorageType>& operator*=(Matrix<StorageType>& matrix, typename StorageType::Scalar scalar) noexcept(
	noexcept(matrix.get(0, 0)))
{
	for (auto rowIdx = 0u; rowIdx < StorageType::ROWS; ++rowIdx) {
		for (auto columnIdx = 0u; columnIdx < StorageType::COLUMNS; ++columnIdx) {
			matrix.set(rowIdx, columnIdx, matrix.get(rowIdx, columnIdx) * scalar);
		}
	}

	return matrix;
}

template <class StorageType>
inline [[nodiscard]] Matrix<StorageType> operator*(const Matrix<StorageType>& matrix, typename StorageType::Scalar scalar)
	noexcept(noexcept(std::declval<Matrix<StorageType>&>() *= scalar))
{
	auto result = matrix;
	result *= scalar;
	return result;
}

template <class StorageType>
inline [[nodiscard]] Matrix<StorageType> operator*(typename StorageType::Scalar scalar, const Matrix<StorageType>& matrix)
	noexcept(noexcept(std::declval<Matrix<StorageType>&>() *= scalar))
{
	auto result = matrix;
	result *= scalar;
	return result;
}

template <class StorageType>
inline Matrix<StorageType>& operator/=(Matrix<StorageType>& matrix, typename StorageType::Scalar scalar) noexcept(
	noexcept(matrix.get(0, 0)))
{
	for (auto rowIdx = 0u; rowIdx < StorageType::ROWS; ++rowIdx) {
		for (auto columnIdx = 0u; columnIdx < StorageType::COLUMNS; ++columnIdx) {
			matrix.set(rowIdx, columnIdx, matrix.get(rowIdx, columnIdx) / scalar);
		}
	}

	return matrix;
}

template <class StorageType>
inline [[nodiscard]] Matrix<StorageType> operator/(const Matrix<StorageType>& matrix, typename StorageType::Scalar scalar)
	noexcept(noexcept(std::declval<Matrix<StorageType>&>() /= scalar))
{
	auto result = matrix;
	result /= scalar;
	return result;
}

template <class StorageType>
inline [[nodiscard]] Matrix<StorageType> operator/(typename StorageType::Scalar scalar, const Matrix<StorageType>& matrix)
	noexcept(noexcept(std::declval<Matrix<StorageType>&>() /= scalar))
{
	auto result = matrix;
	result /= scalar;
	return result;
}

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_MATRIXOPS_HPP__ */
