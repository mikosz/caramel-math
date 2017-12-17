#ifndef CARAMELMATH_ASSERTERRORHANDLER_HPP__
#define CARAMELMATH_ASSERTERRORHANDLER_HPP__

#include <cassert>

namespace caramel_math {

struct AssertErrorHandler {
	
	static constexpr auto THROWS_EXCEPTIONS = false;

	static void invalidAccess([[maybe_unused]] size_t row, [[maybe_unused]] size_t column) {
		assert(!"Invalid matrix data access");
	}

};

} // namespace caramel_math

#endif /* CARAMELMATH_ASSERTERRORHANDLER_HPP__ */
