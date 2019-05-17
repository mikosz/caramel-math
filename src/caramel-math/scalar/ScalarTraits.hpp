#ifndef CARAMELMATH_SCALAR_SCALARTRAITS_HPP__
#define CARAMELMATH_SCALAR_SCALARTRAITS_HPP__

#include "../setup.hpp"

namespace caramel_math::scalar {

template <class T>
struct BasicScalarTraits {
	using Scalar = T;
	static constexpr auto ZERO = T(0);
	static constexpr auto ONE = T(1);

	static constexpr bool equal(T lhs, T rhs) noexcept {
		return lhs == rhs;
	}
};

template <>
struct BasicScalarTraits<float> {
	using Scalar = float;
	static constexpr auto ZERO = 0.0f;
	static constexpr auto ONE = 1.0f;
	static constexpr auto EPSILON = FLOAT_EPSILON;

	static constexpr bool equal(float lhs, float rhs) noexcept {
		return lhs <= rhs + EPSILON && lhs >= rhs - EPSILON;
	}
};

} // namespace caramel_math::scalar

#endif /* CARAMELMATH_SCALAR_SCALARTRAITS_HPP__ */
