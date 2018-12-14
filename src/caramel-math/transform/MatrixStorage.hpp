#ifndef CARAMELMATH_TRANSFORM_MATRIXSTORAGE_HPP__
#define CARAMELMATH_TRANSFORM_MATRIXSTORAGE_HPP__

#include "caramel-math/matrix/Matrix.template.hpp"

namespace caramel_math {
namespace transform {

template <class MatrixType>
class MatrixStorage final {
public:

	static_assert(MatrixType::ROWS == 4);
	static_assert(MatrixType::COLUMNS == 4);

	constexpr MatrixStorage() :
		matrix_(MatrixType::IDENTITY)
	{
	}

	

private:

	MatrixType matrix_;

};

} // namespace transform
} // namespace caramel_math

#endif /* CARAMELMATH_TRANSFORM_MATRIXSTORAGE_HPP__ */
