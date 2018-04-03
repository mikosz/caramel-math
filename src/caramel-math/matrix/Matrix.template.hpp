#ifndef CARAMELMATH_MATRIX_MATRIX_TEMPLATE_HPP__
#define CARAMELMATH_MATRIX_MATRIX_TEMPLATE_HPP__

#include "storage-traits.hpp"
#include "matrix-coordinates.hpp"
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

		for (auto rowIdx = Row(0); rowIdx.value() < ROWS; ++rowIdx) {
			for (auto columnIdx = Column(0); columnIdx.value() < COLUMNS; ++columnIdx) {
				set(rowIdx, columnIdx, other.get(rowIdx, columnIdx));
			}
		}
		
		return *this;
	}

	using StorageType::get;

	typename StorageType::GetReturnType get(Column column, Row row) const
		noexcept(noexcept(get(row, column)))
	{
		return get(row, column);
	}

	using StorageType::set;

	void set(Column column, Row row, Scalar scalar) noexcept(noexcept(set(row, column, scalar))) {
		return set(row, column, std::move(scalar));
	}

	Matrix& transpose() noexcept {
		static_assert(ROWS == COLUMNS, "Can't transpose self for non-square matrices");
		for (auto rowIdx = Row(0); rowIdx.value() < ROWS; ++rowIdx) {
			for (auto columnIdx = Column(0); columnIdx.value() < rowIdx.value(); ++columnIdx) {
				auto stored = get(rowIdx, columnIdx);
				set(rowIdx, columnIdx, get(Row(columnIdx.value()), Column(rowIdx.value())));
				set(Row(columnIdx.value()), Column(rowIdx.value()), stored);
			}
		}

		return *this;
	}

	const Storage& storage() const noexcept {
		return static_cast<const StorageType&>(*this);
	}

};

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_MATRIX_TEMPLATE_HPP__ */
