#ifndef CARAMELMATH_MATRIX_VIEWSTORAGE_HPP__
#define CARAMELMATH_MATRIX_VIEWSTORAGE_HPP__

#include <tuple>

#include "../setup.hpp"
#include "Matrix.hpp"

namespace caramel_math::matrix {

template <
	class ViewedMatrixType,
	class ModifierFuncType
	>
class ViewStorage {
public:

	using ViewedMatrix = ViewedMatrixType;

	using ModifierFunc = ModifierFuncType;

	using Scalar = typename ViewedMatrixType::Scalar;

	static constexpr auto ROWS = ViewedMatrix::COLUMNS;

	static constexpr auto COLUMNS = ViewedMatrix::ROWS;

	static constexpr auto MUTABLE = ViewedMatrix::MUTABLE;

	ViewStorage(ViewedMatrix& viewedMatrix, ModifierFunc modifierFunc = ModifierFunc()) :
		viewedMatrix_(viewedMatrix),
		modifierFunc_(std::move(modifierFunc))
	{
	}

	auto& get(size_t row, size_t column) noexcept(noexcept(viewedMatrix_.get(row, column))) {
		auto modifiedRow = size_t();
		auto modifiedColumn = size_t();
		std::tie(modifiedRow, modifiedColumn) = modifierFunc_(row, column);
		return viewedMatrix_.get(modifiedRow, modifiedColumn);
	}

private:

	ViewedMatrix& viewedMatrix_;

	ModifierFunc modifierFunc_;

};

namespace detail {

struct TransposedModifierFunc {

	std::tuple<size_t, size_t> operator()(size_t row, size_t column) noexcept {
		return { column, row };
	}

};

} // namespace detail

template <class ViewedMatrixType>
using TransposedViewStorage = ViewStorage<ViewedMatrixType, detail::TransposedModifierFunc>;

template <class ViewedMatrixType>
inline auto viewTransposed(ViewedMatrixType& matrix) noexcept {
	return Matrix<TransposedViewStorage<ViewedMatrixType>>(matrix);
}

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_VIEWSTORAGE_HPP__ */
