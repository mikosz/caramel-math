#ifndef CARAMELMATH_SETUP_HPP__
#define CARAMELMATH_SETUP_HPP__

namespace caramel_math {

#ifdef _NDEBUG
constexpr auto IS_DEBUG = false;
#else
constexpr auto IS_DEBUG = true;
#endif /* _NDEBUG */

constexpr auto RUNTIME_CHECKS = IS_DEBUG;

constexpr auto FLOAT_EPSILON = 0.0001f;

} // namespace caramel_math

#endif /* CARAMELMATH_SETUP_HPP__ */
