#ifndef CARAMELMATH_MATRIX_MATRIXCOORDINATES_HPP__
#define CARAMELMATH_MATRIX_MATRIXCOORDINATES_HPP__

#include "../scalar/Value.hpp"

namespace caramel_math::matrix {

// TODO: size_t -> int?
struct Row : private scalar::Value<Row, size_t> {

	constexpr Row() = default;

	constexpr explicit Row(size_t value) :
		Value<Row, size_t>(value)
	{
	}

	constexpr auto value() const noexcept {
		return Value<Row, size_t>::value();
	}

};

struct Column : private scalar::Value<Column, size_t> {

	constexpr Column() = default;

	constexpr explicit Column(size_t value) :
		Value<Column, size_t>(value)
	{
	}

	constexpr auto value() const noexcept {
		return Value<Column, size_t>::value();
	}

};

namespace literals {

constexpr caramel_math::matrix::Row operator""_row(unsigned long long int row) {
	return caramel_math::matrix::Row(size_t(row));
}

constexpr caramel_math::matrix::Column operator""_col(unsigned long long int column) {
	return caramel_math::matrix::Column(size_t(column));
}

} // namespace literals

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_MATRIXCOORDINATES_HPP__ */
