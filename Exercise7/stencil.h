#ifndef STENCIL_H
#define STENCIL_H

#include "jacobi.h"
#include "iteration.h"

// ==============================================================================
// define types
//using cell_t = float;
using cell_t = std::array<float, 2>;
using cell_value_t = cell_t::value_type;

template<size_t Dim>
using grid_t = stencil::grid_t<cell_t, Dim>;

template<size_t Dim>
using bounds_t = stencil::bounds_t<cell_t, Dim>;

// stencil code using jacobi iteration
using stencil_code = stencil::stencil_iteration<stencil::jacobi>;

constexpr auto default_epsilon = 1;

#endif // STENCIL_H
