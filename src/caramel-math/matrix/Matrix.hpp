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

	// Inherit constructors
	using StorageType::StorageType;

	using StorageType::get;

	using StorageType::set;

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

template <class LHSStorageType, class RHSStorageType>
inline bool operator==(
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
inline bool operator!=(
	const Matrix<LHSStorageType>& lhs,
	const Matrix<RHSStorageType>& rhs
	) noexcept(noexcept(lhs == rhs))
{
	return !(lhs == rhs);
}

template <class LHSStorageType, class RHSStorageType>
inline auto operator*(
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
inline Matrix<StorageType> operator*(const Matrix<StorageType>& matrix, typename StorageType::Scalar scalar)
	noexcept(noexcept(std::declval<Matrix<StorageType>&>() *= scalar))
{
	auto result = matrix;
	result *= scalar;
	return result;
}

template <class StorageType>
inline Matrix<StorageType> operator*(typename StorageType::Scalar scalar, const Matrix<StorageType>& matrix)
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
inline Matrix<StorageType> operator/(const Matrix<StorageType>& matrix, typename StorageType::Scalar scalar)
	noexcept(noexcept(std::declval<Matrix<StorageType>&>() /= scalar))
{
	auto result = matrix;
	result /= scalar;
	return result;
}

template <class StorageType>
inline Matrix<StorageType> operator/(typename StorageType::Scalar scalar, const Matrix<StorageType>& matrix)
	noexcept(noexcept(std::declval<Matrix<StorageType>&>() /= scalar))
{
	auto result = matrix;
	result /= scalar;
	return result;
}

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_MATRIX_HPP__ */
