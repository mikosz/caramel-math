#ifndef CARAMELMATH_MATRIX_MATRIX_HPP__
#define CARAMELMATH_MATRIX_MATRIX_HPP__

#include "storage-traits.hpp"

namespace caramel_math::matrix {

template <class StorageType>
class Matrix final : StorageType {
public:

	using Storage = StorageType;

	using StorageType::StorageType;

	using typename StorageType::Scalar;

	using StorageType::ROWS;

	using StorageType::COLUMNS;

	decltype(auto) get(size_t row, size_t column) const noexcept(
		noexcept(std::declval<StorageType&>().get(row, column)))
	{
		return this->StorageType::get(row, column);
	}

	void set(size_t row, size_t column, Scalar scalar) noexcept(
		noexcept(std::declval<StorageType&>().set(row, column, scalar)))
	{
		return this->StorageType::set(row, column, std::move(scalar));
	}

};

template <class LHSStorageType, class RHSStorageType>
inline auto operator*(
	const Matrix<LHSStorageType>& lhs,
	const Matrix<RHSStorageType>& rhs
	) noexcept(noexcept(lhs.get(0, 0)) && noexcept(rhs.get(0, 0)))
{
	static_assert(LHSStorageType::COLUMNS == RHSStorageType::ROWS, "Incompatible matrix sizes for multiplication");
	using ResultType = BinaryOperatorResultType<LHSStorageType, RHSStorageType, LHSStorageType::ROWS, RHSStorageType::COLUMNS>;

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

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_MATRIX_HPP__ */
