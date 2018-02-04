#ifndef CARAMELMATH_MATRIX_MATRIX_HPP__
#define CARAMELMATH_MATRIX_MATRIX_HPP__

#include "storage-traits.hpp"
#include "matrixfwd.hpp"

namespace caramel_math::matrix {

template <class StorageType>
class Matrix final : StorageType {
public:

	using Storage = StorageType;

	using typename StorageType::Scalar;

	using StorageType::ROWS;

	using StorageType::COLUMNS;

	static Matrix ZERO;

	static Matrix IDENTITY;

	Matrix() = default;
	Matrix(const Matrix& other) = default;
	Matrix(Matrix&& other) = default;
	Matrix& operator=(const Matrix& other) = default;
	Matrix& operator=(Matrix&& other) = default;

	using StorageType::StorageType;

	// Conversion from compatible matrix types
	// TODO: this should probably be implemented in storage also (not in here)
	template <class OtherStorageType>
	explicit Matrix(const Matrix<OtherStorageType>& other) {
		*this = other;
	}

	template <class OtherStorageType>
	Matrix& operator=(const Matrix<OtherStorageType>& other) {
		static_assert(ROWS == Matrix<OtherStorageType>::ROWS);
		static_assert(COLUMNS == Matrix<OtherStorageType>::COLUMNS);

		for (auto rowIdx = 0u; rowIdx < ROWS; ++rowIdx) {
			for (auto columnIdx = 0u; columnIdx < COLUMNS; ++columnIdx) {
				set(rowIdx, columnIdx, other.get(rowIdx, columnIdx));
			}
		}
		
		return *this;
	}

	// Inherit get/set functions
	using StorageType::get;

	using StorageType::set;

	Matrix& transpose() noexcept {
		static_assert(ROWS == COLUMNS, "Can't assign transposed matrix to self for non-square matrices");
		for (auto rowIdx = 0u; rowIdx < ROWS; ++rowIdx) {
			for (auto columnIdx = 0u; columnIdx < rowIdx; ++columnIdx) {
				auto stored = get(rowIdx, columnIdx);
				set(rowIdx, columnIdx, get(columnIdx, rowIdx));
				set(columnIdx, rowIdx, stored);
			}
		}

		return *this;
	}

	const Storage& storage() const noexcept {
		return static_cast<const StorageType&>(*this);
	}

};

namespace detail {

template <class StorageType>
Matrix<StorageType> zeroMatrix() {
	auto zero = Matrix<StorageType>();
	for (auto rowIdx = 0u; rowIdx < StorageType::ROWS; ++rowIdx) {
		for (auto columnIdx = 0u; columnIdx < StorageType::COLUMNS; ++columnIdx) {
			zero.set(rowIdx, columnIdx, StorageType::ScalarTraits::ZERO);
		}
	}
	return zero;
}

template <class StorageType>
Matrix<StorageType> identityMatrix() {
	auto zero = Matrix<StorageType>();
	for (auto rowIdx = 0u; rowIdx < StorageType::ROWS; ++rowIdx) {
		for (auto columnIdx = 0u; columnIdx < StorageType::COLUMNS; ++columnIdx) {
			if (rowIdx == columnIdx) {
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

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_MATRIX_HPP__ */
