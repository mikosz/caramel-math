#ifndef CARAMELMATH_MATRIX_MATRIX_IMPLEMENTATION_HPP__
#define CARAMELMATH_MATRIX_MATRIX_IMPLEMENTATION_HPP__

#include <iosfwd>
#include <optional>

#include "Matrix.template.hpp"
#include "ViewStorage.hpp"
#include "storage-traits.hpp"

namespace caramel_math::matrix {

// -- const variants initialisation

namespace detail {

template <class StorageType>
Matrix<StorageType> zeroMatrix() {
	auto zero = Matrix<StorageType>();
	for (auto rowIdx = Row(0); rowIdx.value() < StorageType::ROWS; ++rowIdx) {
		for (auto columnIdx = Column(0); columnIdx.value() < StorageType::COLUMNS; ++columnIdx) {
			zero.set(rowIdx, columnIdx, StorageType::ScalarTraits::ZERO);
		}
	}
	return zero;
}

template <class StorageType>
Matrix<StorageType> identityMatrix() {
	auto zero = Matrix<StorageType>();
	for (auto rowIdx = Row(0); rowIdx.value() < StorageType::ROWS; ++rowIdx) {
		for (auto columnIdx = Column(0); columnIdx.value() < StorageType::COLUMNS; ++columnIdx) {
			if (rowIdx.value() == columnIdx.value()) {
				zero.set(rowIdx, columnIdx, StorageType::ScalarTraits::ONE);
			} else {
				zero.set(rowIdx, columnIdx, StorageType::ScalarTraits::ZERO);
			}
		}
	}
	return zero;
}

} // namespace detail

template <class StorageType>
Matrix<StorageType> Matrix<StorageType>::ZERO = detail::zeroMatrix<StorageType>();

template <class StorageType>
Matrix<StorageType> Matrix<StorageType>::IDENTITY = detail::identityMatrix<StorageType>();

// -- operators

template <class LHSStorageType, class RHSStorageType>
inline [[nodiscard]] bool operator==(
	const Matrix<LHSStorageType>& lhs,
	const Matrix<RHSStorageType>& rhs
	) noexcept(noexcept(lhs.get(Row(0), Column(0))) && noexcept(rhs.get(Row(0), Column(0))))
{
	static_assert(
		LHSStorageType::COLUMNS == RHSStorageType::COLUMNS &&
		LHSStorageType::ROWS == RHSStorageType::ROWS,
		"Incompatible matrix sizes for equality test"
		);

	for (auto rowIdx = Row(0); rowIdx.value() < LHSStorageType::ROWS; ++rowIdx) {
		for (auto columnIdx = Column(0); columnIdx.value() < LHSStorageType::COLUMNS; ++columnIdx) {
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
	) noexcept(noexcept(lhs.get(Row(0), Column(0))) && noexcept(rhs.get(Row(0), Column(0))))
{
	static_assert(LHSStorageType::COLUMNS == RHSStorageType::ROWS, "Incompatible matrix sizes for multiplication");
	using ResultStorageType =
		BinaryOperatorResultType<LHSStorageType, RHSStorageType, LHSStorageType::ROWS, RHSStorageType::COLUMNS>;
	using ResultType = Matrix<ResultStorageType>;

	auto result = ResultType();

	for (auto rowIdx = Row(0); rowIdx.value() < LHSStorageType::ROWS; ++rowIdx) {
		for (auto columnIdx = Column(0); columnIdx.value() < RHSStorageType::COLUMNS; ++columnIdx) {
			auto dot = ResultType::ScalarTraits::ZERO;
			for (auto dotIdx = 0u; dotIdx < LHSStorageType::COLUMNS; ++dotIdx) {
				dot += lhs.get(rowIdx, Column(dotIdx)) * rhs.get(Row(dotIdx), columnIdx);
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
	static_assert(LHSStorageType::COLUMNS == RHSStorageType::COLUMNS, "Incompatible matrix sizes for assign-multiplication");
	lhs = lhs * rhs;
	return lhs;
}

template <class StorageType>
inline Matrix<StorageType>& operator*=(Matrix<StorageType>& matrix, typename StorageType::Scalar scalar) noexcept(
	noexcept(matrix.get(Row(0), Column(0))))
{
	for (auto rowIdx = Row(0); rowIdx.value() < StorageType::ROWS; ++rowIdx) {
		for (auto columnIdx = Column(0); columnIdx.value() < StorageType::COLUMNS; ++columnIdx) {
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
	noexcept(matrix.get(Row(0), Column(0))))
{
	for (auto rowIdx = Row(0); rowIdx.value() < StorageType::ROWS; ++rowIdx) {
		for (auto columnIdx = Column(0); columnIdx.value() < StorageType::COLUMNS; ++columnIdx) {
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
	
	for (auto rowIdx = Row(0); rowIdx.value() < Matrix<StorageType>::ROWS; ++rowIdx) {
		os << "{ ";

		for (auto columnIdx = Column(0); columnIdx.value() < Matrix<StorageType>::COLUMNS; ++columnIdx) {
			os << matrix.get(rowIdx, columnIdx);

			if (columnIdx.value() != Matrix<StorageType>::COLUMNS - 1) {
				os << ", ";
			}
		}

		if (rowIdx.value() == Matrix<StorageType>::ROWS - 1) {
			os << " }";
		} else {
			os << " }, ";
		}
	}

	os << " }";

	return os;
}

// -- free functions

template <class StorageType>
inline [[nodiscard]] auto transposed(const Matrix<StorageType>& matrix) noexcept {
	return Matrix<TransposedStorageType<StorageType>>(viewTransposed(matrix));
}

template <class ViewedMatrixType>
inline [[nodiscard]] const auto& transposed(
	const Matrix<TransposedViewStorage<ViewedMatrixType>>& transposedMatrixView
	) noexcept
{
	return transposedMatrixView.storage().viewedMatrix();
}

template <class StorageType>
inline auto determinant(const Matrix<StorageType>& matrix) noexcept {
	using MatrixType = Matrix<StorageType>;
	static_assert(MatrixType::ROWS == MatrixType::COLUMNS, "Determinant only defined for square matrices");

	if constexpr (MatrixType::ROWS > 1) {
		auto result = MatrixType::Scalar(0);
		for (auto columnIndex = Column(0); columnIndex.value() < MatrixType::COLUMNS; ++columnIndex) {
			const auto absElement =
				matrix.get(Row(0), columnIndex) * determinant(viewSubmatrix(matrix, Row(0), columnIndex));
			if (columnIndex % 2 == Column(0)) {
				result += absElement;
			} else {
				result -= absElement;
			}
		}
		return result;
	} else {
		return matrix.get(Row(0), Column(0));
	}
}

template <class StorageType>
inline auto cofactor(const Matrix<StorageType>& matrix, Row rowIndex, Column columnIndex) noexcept {
	const auto det = determinant(viewSubmatrix(matrix, rowIndex, columnIndex));
	return ((rowIndex.value() + columnIndex.value()) % 2 == 0) ? det : -det;
}

template <class StorageType>
inline auto inverse(const Matrix<StorageType>& matrix) noexcept /* TODO: not really */ {
	using MatrixType = Matrix<StorageType>;
	using Scalar = typename MatrixType::Scalar;
	static_assert(MatrixType::ROWS == MatrixType::COLUMNS, "Inverse only defined for square matrices");

	auto result = std::optional<EffectiveStorageType<MatrixType>>(); // TODO: effective storage type may not work?
	
	const auto det = determinant(matrix);

	if (det != Scalar(0)) {
		const auto detInverse = Scalar(1) / det;

		result = EffectiveStorageType<MatrixType>();

		for (auto rowIndex = Row(0); rowIndex.value() < MatrixType::ROWS; ++rowIndex) {
			for (auto columnIndex = Column(0); columnIndex.value() < MatrixType::COLUMNS; ++columnIndex) {
				result->set(
					rowIndex,
					columnIndex,
					detInverse * cofactor(matrix, Row(columnIndex.value()), Column(rowIndex.value()))
					);
			}
		}
	}

	return result;
}

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_MATRIX_IMPLEMENTATION_HPP__ */
