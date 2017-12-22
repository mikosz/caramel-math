#ifndef CARAMELMATH_MATRIX_VIEWSTORAGE_HPP__
#define CARAMELMATH_MATRIX_VIEWSTORAGE_HPP__

#include <tuple>

#include "../setup.hpp"

namespace caramel_math::matrix {

template <
	class ViewedMatrixType,
	class ModifierFuncType
	>
class ViewStorage {
public:

	using ViewedMatrix = ViewedMatrixType;

	using ModifierFunc = ModifierFuncType;

	ViewStorage(ViewedMatrix& viewedMatrix, ModifierFunc modifierFunc = ModifierFunc()) :
		viewedMatrix_(viewedMatrix),
		modifierFunc_(std::move(modifierFunc))
	{
	}

	auto& get(size_t row, size_t column) {//noexcept(noexcept(ErrorHandler::invalidAccess(0, 0))) {
		auto modifiedRow = size_t();
		auto modifiedColumn = size_t();
		std::tie(modifiedRow, modifiedColumn) = modifierFunc_(row, column);
		return viewedMatrix_.get(modifiedRow, modifiedColumn);
	}

private:

	ViewedMatrix& viewedMatrix_;

	ModifierFunc modifierFunc_;

};

struct TransposedModifierFunc {

	std::tuple<size_t, size_t> operator()(size_t row, size_t column) noexcept {
		return { column, row };
	}

};

template <class ViewedMatrixType>
using TransposedViewStorage = ViewStorage<ViewedMatrixType, TransposedModifierFunc>;

} // namespace caramel_math::matrix

#endif /* CARAMELMATH_MATRIX_VIEWSTORAGE_HPP__ */
