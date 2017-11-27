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
    static auto execute(grid_t<CellType, 1>& grid)
    {
        using error_type = decltype(StencilCodeImpl<1>
            ::template execute<ReadIndex>(grid, 0));

        error_type error = 0;

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
    static auto execute(grid_t<CellType, 2> &grid)
    {
        using error_type = decltype(StencilCodeImpl<2>
            ::template execute<ReadIndex>(grid, 0, 0));

        error_type error = 0;

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
    static auto execute(grid_t<CellType, 3>& grid)
    {
        using error_type = decltype(StencilCodeImpl<3>
            ::template execute<ReadIndex>(grid, 0, 0, 0));

        error_type error = 0;

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
    static auto do_iteration(GridType& grid, const DiffType epsilon)
    {
        using iteration_type = iteration<GridType::dim>;
        using return_type = std::tuple<const int, const bool, const DiffType>;

        static constexpr auto batch_threshold = 1'000'000;
        static constexpr auto batch_size = 5;

        const auto iterations = 2 + [&]()
        {
            if (stencil::size(grid.extents()) < batch_threshold)
            {
                for (auto i = 0u; i < batch_size; ++i)
                {
                    iteration_type::template execute<StencilCodeImpl, 0>(grid);
                    iteration_type::template execute<StencilCodeImpl, 1>(grid);
                }

                return 2 * batch_size;
            }

            return 0;
        }();
        
        iteration_type::template execute<StencilCodeImpl, 0>(grid);
        
        const auto error = iteration_type::template execute<StencilCodeImpl, 1>(grid);

        return return_type(iterations, error < epsilon, error);
    } 
};



}

#endif // ITERATION_H
