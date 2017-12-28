#ifndef CARAMELMATH_MATRIX_MATRIX_HPP__
#define CARAMELMATH_MATRIX_MATRIX_HPP__

namespace caramel_math::matrix {

template <class StorageType>
class Matrix final : StorageType {
public:

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

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_MATRIX_HPP__ */
