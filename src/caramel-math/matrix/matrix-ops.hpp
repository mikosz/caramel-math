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

template <class StorageType>
inline auto cofactor(const Matrix<StorageType>& matrix, size_t rowIndex, size_t columnIndex) noexcept {
	const auto det = determinant(viewSubmatrix(matrix, rowIndex, columnIndex));
	return ((rowIndex + columnIndex) % 2 == 0) ? det : -det;
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

		for (size_t rowIndex = 0; rowIndex < MatrixType::ROWS; ++rowIndex) {
			for (size_t columnIndex = 0; columnIndex < MatrixType::COLUMNS; ++columnIndex) {
				result->set(rowIndex, columnIndex, detInverse * cofactor(matrix, columnIndex, rowIndex));
			}
		}
	}

	return result;
}

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_MATRIXOPS_HPP__ */
