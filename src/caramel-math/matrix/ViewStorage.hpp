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

	using Scalar = typename ViewedMatrix::Scalar;

	static constexpr auto ROWS = ViewedMatrix::COLUMNS;

	static constexpr auto COLUMNS = ViewedMatrix::ROWS;

	ViewStorage(ViewedMatrix& viewedMatrix, ModifierFunc modifierFunc = ModifierFunc()) :
		viewedMatrix_(viewedMatrix),
		modifierFunc_(std::move(modifierFunc))
	{
	}

	decltype(auto) get(size_t row, size_t column) const noexcept(
		noexcept(viewedMatrix_.get(row, column)))
	{
		auto modifiedRow = size_t();
		auto modifiedColumn = size_t();
		std::tie(modifiedRow, modifiedColumn) = modifierFunc_(row, column);
		return viewedMatrix_.get(modifiedRow, modifiedColumn);
	}

	void set(size_t row, size_t column, Scalar scalar) noexcept(
		noexcept(viewedMatrix_.set(row, column, scalar)))
	{
		auto modifiedRow = size_t();
		auto modifiedColumn = size_t();
		std::tie(modifiedRow, modifiedColumn) = modifierFunc_(row, column);
		return viewedMatrix_.set(modifiedRow, modifiedColumn, std::move(scalar));
	}

	ViewedMatrix& viewedMatrix() noexcept {
		return viewedMatrix_;
	}

	const ViewedMatrix& viewedMatrix() const noexcept {
		return viewedMatrix_;
	}

private:

	ViewedMatrix& viewedMatrix_;

	ModifierFunc modifierFunc_;

};

namespace detail {

struct TransposedModifierFunc {

	std::tuple<size_t, size_t> operator()(size_t row, size_t column) const noexcept {
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
