#ifndef CARAMELMATH_MATRIX_MATRIXOPS_HPP__
#define CARAMELMATH_MATRIX_MATRIXOPS_HPP__

#include <iosfwd>
#include <optional>

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

#endif /* CARAMELMATH_MATRIX_MATRIXOPS_HPP__ */
