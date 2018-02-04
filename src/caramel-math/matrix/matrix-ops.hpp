#ifndef CARAMELMATH_MATRIX_MATRIXOPS_HPP__
#define CARAMELMATH_MATRIX_MATRIXOPS_HPP__

#include <iosfwd>

#include "Matrix.hpp"
#include "ViewStorage.hpp"
#include "storage-traits.hpp"

namespace caramel_math::matrix {

template <class StorageType>
inline [[nodiscard]] auto transposed(const Matrix<StorageType>& matrix) {
	return Matrix<TransposedStorageType<StorageType>>(viewTransposed(matrix));
}

template <class ViewedMatrixType>
inline [[nodiscard]] const auto& transposed(const Matrix<TransposedViewStorage<ViewedMatrixType>>& transposedMatrixView) {
	return transposedMatrixView.storage().viewedMatrix();
}

template <class StorageType>
inline auto determinant(const Matrix<StorageType>& matrix) noexcept {
	using MatrixType = Matrix<StorageType>;
	static_assert(MatrixType::ROWS == MatrixType::COLUMNS, "Determinant only defined for square matrices");

	if constexpr (MatrixType::ROWS > 1) {
		auto result = MatrixType::Scalar(0);
		for (size_t columnIndex = 0; columnIndex < MatrixType::COLUMNS; ++columnIndex) {
			const auto absElement = matrix.get(0, columnIndex) * determinant(viewSubmatrix(matrix, 0, columnIndex));
			if (columnIndex % 2 == 0) {
				result += absElement;
			} else {
				result -= absElement;
			}
		}
		return result;
	} else {
		return matrix.get(0, 0);
	}
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

template <class StorageType>
inline std::ostream& operator<<(std::ostream& os, const Matrix<StorageType>& matrix) noexcept {
	os << "{ ";
	
	for (auto rowIdx = 0; rowIdx < Matrix<StorageType>::ROWS; ++rowIdx) {
		os << "{ ";

		for (auto columnIdx = 0; columnIdx < Matrix<StorageType>::COLUMNS; ++columnIdx) {
			os << matrix.get(rowIdx, columnIdx);

			if (columnIdx != Matrix<StorageType>::COLUMNS - 1) {
				os << ", ";
			}
		}

		if (rowIdx == Matrix<StorageType>::ROWS - 1) {
			os << " }";
		} else {
			os << " }, ";
		}
	}

	os << " }";

	return os;
}

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_MATRIXOPS_HPP__ */
