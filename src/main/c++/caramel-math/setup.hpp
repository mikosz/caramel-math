#ifndef CARAMELMATH_SETUP_HPP__
#define CARAMELMATH_SETUP_HPP__

namespace caramel_math {

#ifdef _NDEBUG
const auto IS_DEBUG = false;
#else
const auto IS_DEBUG = true;
#endif /* _NDEBUG */

const auto RUNTIME_CHECKS = IS_DEBUG;

} // namespace caramel_math

#endif /* CARAMELMATH_SETUP_HPP__ */
