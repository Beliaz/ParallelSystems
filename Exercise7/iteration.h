#ifndef ITERATION_H
#define ITERATION_H
#include "grid.h"

namespace stencil
{
    
template<size_t Dim>
struct iteration;

template<>
struct iteration<1>
{
    template<template <size_t Dim> class StencilCodeImpl, size_t ReadIndex, class CellType>
    static auto execute(stencil::grid_t<CellType, 1>& grid)
    {
        auto error = 0.0f;

        #pragma omp parallel for reduction (+ : error)
        for (auto i = 1u; i < grid.extents()[0] - 1; ++i)
        {
            error += StencilCodeImpl<1>::template execute<ReadIndex>(grid, i);
        }

        return error;
    }
};

template<>
struct iteration<2>
{
    template<template <size_t Dim> class StencilCodeImpl, size_t ReadIndex, class CellType>
    static auto execute(stencil::grid_t<CellType, 2> &grid)
    {
        auto error = 0.0f;

        #pragma omp parallel for reduction (+ : error)
        for (auto y = 1u; y < grid.extents()[1] - 1; ++y)
        {
            for (auto x = 1u; x < grid.extents()[0] - 1; ++x)
            {
                error += StencilCodeImpl<2>::template execute<ReadIndex>(grid, x, y);
            }
        }

        return error;
    }
};

template<>
struct iteration<3>
{
    template<template <size_t Dim> class StencilCodeImpl, size_t ReadIndex, class CellType>
    static auto execute(stencil::grid_t<CellType, 3>& grid)
    {
        auto error = 0.0f;

        #pragma omp parallel for reduction (+ : error)
        for (auto z = 1u; z < grid.extents()[2] - 1; ++z)
        {
            for (auto y = 1u; y < grid.extents()[1] - 1; ++y)
            {
                for (auto x = 1u; x < grid.extents()[0] - 1; ++x)
                {
                    error += StencilCodeImpl<3>::template execute<ReadIndex>(grid, x, y, z);
                }
            }
        }

        return error;
    }
};

template<template <size_t Dim> class StencilCodeImpl>
struct stencil_iteration
{
    template<class GridType, class DiffType>
    static std::tuple<const int, const bool, const DiffType>
    do_iteration(GridType& grid, const DiffType epsilon)
    {
        using iteration_type = iteration<GridType::dim>;

        const auto first_error = iteration_type::template execute<StencilCodeImpl, 0>(grid);
        if (first_error < epsilon)
            return { 1, true, first_error };
        
        const auto second_error = iteration_type::template execute<StencilCodeImpl, 1>(grid);
        if (second_error < epsilon)
            return { 2, true, second_error };

        return { 2, false, second_error };
    } 
};



}

#endif // ITERATION_H
