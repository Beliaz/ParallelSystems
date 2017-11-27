#ifndef ITERATION_H
#define ITERATION_H

#include "grid.h"

#include <numeric>

namespace stencil
{
    
static constexpr auto error_buffer_size = 8;

template<class ErrorType>
using error_buffer = std::array<ErrorType, error_buffer_size>;


template<size_t Dim>
struct iteration;

template<>
struct iteration<1>
{
    template<template <size_t Dim> class StencilCodeImpl, class CellType>
    static auto execute(grid_t<CellType, 1>& source, grid_t<CellType, 1>& target)
    {
        using error_type = decltype(StencilCodeImpl<1>
            ::template execute(source, 0));

        error_type error = 0;

        #pragma omp parallel for reduction(+: error)
        for (auto i = 1u; i < source.extents()[0] - 1; ++i)
        {
            const auto new_value = StencilCodeImpl<1>
                ::template execute(source, i);

            error += std::abs(new_value - source.at(i));

            target.at(i) = new_value;
        }

        return error;
    }
};

template<>
struct iteration<2>
{
    template<template <size_t Dim> class StencilCodeImpl, class CellType>
    static auto execute(grid_t<CellType, 2>& source, grid_t<CellType, 2>& target)
    {
        using error_type = decltype(StencilCodeImpl<2>
            ::template execute(source, 0, 0));

        error_type error = 0;

        #pragma omp parallel for reduction(+: error)
        for (auto y = 1u; y < source.extents()[1] - 1; ++y)
        {
            for (auto x = 1u; x < source.extents()[0] - 1; ++x)
            {
                const auto new_value = StencilCodeImpl<2>
                    ::template execute(source, x, y);

                error += std::abs(new_value - source.at({ x, y }));

                target.at({ x, y }) = new_value;
            }
        }

        return error;
    }
};

template<>
struct iteration<3>
{
    template<template <size_t Dim> class StencilCodeImpl, class CellType>
    static auto execute(grid_t<CellType, 3>& source, grid_t<CellType, 3>& target)
    {
        using error_type = decltype(StencilCodeImpl<3>
            ::template execute(source, 0, 0, 0));

        error_type error = 0;
        
        #pragma omp parallel for reduction(+: error)
        for (auto z = 1u; z < source.extents()[2] - 1; ++z)
        {
            for (auto y = 1u; y < source.extents()[1] - 1; ++y)
            {
                for (auto x = 1u; x < source.extents()[0] - 1; ++x)
                {
                    const auto new_value = StencilCodeImpl<3>
                        ::template execute(source, x, y, z);

                    error += std::abs(new_value - source.at({ x, y, z }));

                    target.at({ x, y, z }) = new_value;
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
    static auto do_iteration(std::array<GridType, 2>& grids, const DiffType epsilon)
    {
        using iteration_type = iteration<GridType::dim>;
        using return_type = std::tuple<const int, const bool, const DiffType>;

        static constexpr auto batch_threshold = 1000000;
        static constexpr auto batch_size = 5;

        const auto iterations = 2 + [&]()
        {
            if (stencil::size(grids[0].extents()) < batch_threshold)
            {
                for (auto i = 0u; i < batch_size; ++i)
                {
                    iteration_type::template execute<StencilCodeImpl>(grids[0], grids[1]);
                    iteration_type::template execute<StencilCodeImpl>(grids[1], grids[0]);
                }

                return 2 * batch_size;
            }

            return 0;
        }();
        
        iteration_type::template execute<StencilCodeImpl>(grids[0], grids[1]);
        
        const auto error = iteration_type::template execute<StencilCodeImpl>(grids[1], grids[0]);

        return return_type(iterations, error < epsilon, error);
    } 
};



}

#endif // ITERATION_H
