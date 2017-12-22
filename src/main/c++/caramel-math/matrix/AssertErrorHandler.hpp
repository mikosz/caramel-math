#ifndef CARAMELMATH_MATRIX_ASSERTERRORHANDLER_HPP__
#define CARAMELMATH_MATRIX_ASSERTERRORHANDLER_HPP__

#include <cassert>

namespace caramel_math::matrix {

template <class ScalarType>
struct AssertErrorHandler final {
	
	static ScalarType& invalidAccess([[maybe_unused]] size_t row, [[maybe_unused]] size_t column) noexcept {
		assert(!"Invalid matrix data access");
		
		static auto returnValue = ScalarType();
		return returnValue;
	}

};

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_ASSERTERRORHANDLER_HPP__ */
