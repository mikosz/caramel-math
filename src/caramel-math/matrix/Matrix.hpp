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

// -- operators

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

#endif /* CARAMELMATH_MATRIX_MATRIX_HPP__ */
