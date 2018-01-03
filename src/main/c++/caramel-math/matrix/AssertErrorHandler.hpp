#ifndef CARAMELMATH_MATRIX_ASSERTERRORHANDLER_HPP__
#define CARAMELMATH_MATRIX_ASSERTERRORHANDLER_HPP__

#include <cassert>
#include <type_traits>

namespace caramel_math::matrix {

struct AssertErrorHandler final {
	
	template <class ReturnType>
	static ReturnType invalidAccess([[maybe_unused]] size_t row, [[maybe_unused]] size_t column) noexcept {
		assert(!"Invalid matrix data access");
		
		static auto returnValue = std::decay_t<ReturnType>();
		return returnValue;
	}

};

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_ASSERTERRORHANDLER_HPP__ */
