#ifndef CARAMELMATH_SIMD_DETAIL_VISUALCPPFLOAT4_HPP__
#define CARAMELMATH_SIMD_DETAIL_VISUALCPPFLOAT4_HPP__

#if defined(_MSC_VER)

#include <xmmintrin.h>

namespace caramel_math::simd::detail {

using Float4 = __m128;

inline Float4 load(Float4& data, float xyzw[4]) {
	data = _mm_load_ps(xyzw);
}

inline void store(float xyzw[4], const Float4& data) {
	_mm_store_ps(xyzw, data);
}

inline Float4 add(Float4 lhs, Float4 rhs) noexcept {
	return _mm_add_ps(lhs, rhs);
}

inline Float4 multiply(Float4 lhs, Float4 rhs) noexcept {
	return _mm_mul_ps(lhs, rhs);
}

} // namespace caramel_math::simd::detail

#endif /* defined(_MSC_VER) */

#endif /* CARAMELMATH_SIMD_DETAIL_VISUALCPPFLOAT4_HPP__ */
