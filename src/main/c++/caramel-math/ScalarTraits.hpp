#ifndef CARAMELMATH_SCALARTRAITS_HPP__
#define CARAMELMATH_SCALARTRAITS_HPP__

#include <cmath>

#include "setup.hpp"

namespace caramel_math {

template <class T>
struct ScalarTraits {
	static constexpr auto ZERO = T(0);
	static constexpr auto ONE = T(1);

	static constexpr bool equal(T lhs, T rhs) noexcept {
		return lhs == rhs;
	}
};

struct ScalarTraits<float> {
	static constexpr auto ZERO = 0.0f;
	static constexpr auto ONE = 1.0f;
	static constexpr auto EPSILON = FLOAT_EPSILON;

	static constexpr bool equal(float lhs, float rhs) noexcept {
		return std::fabsf(lhs - rhs) < EPSILON;
	}
};

} // namespace caramel_math

#endif /* CARAMELMATH_SCALARTRAITS_HPP__ */
