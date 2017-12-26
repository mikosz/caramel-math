#ifndef CARAMELMATH_MATRIX_MATRIX_HPP__
#define CARAMELMATH_MATRIX_MATRIX_HPP__

#include <type_traits>

namespace caramel_math::matrix {

template <class StorageType>
class Matrix final : StorageType {
public:

	using StorageType::StorageType;

	using typename StorageType::Scalar;

	using StorageType::ROWS;

	using StorageType::COLUMNS;

	using StoargeType::MUTABLE;

	Reference get(size_t row, size_t column) noexcept(noexcept(std::declval<StorageType&>().get(row, column)))
	{
		return this->StorageType::get(row, column);
	}

	std::add_const_t<Reference> get(size_t row, size_t column) const
		noexcept(noexcept(std::declval<Matrix&>().get(row, column)))
	{
		return const_cast<Matrix&>(*this).get(row, column);
	}

};

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_MATRIX_HPP__ */
