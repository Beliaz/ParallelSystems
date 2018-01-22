#ifndef ITERATION_H
#define ITERATION_H

#include "grid.h"
#include "requirements.h"

#include <numeric>
#include <cmath>

namespace stencil_code
{
    
template<size_t Dim>
struct iteration;

template<>
struct iteration<1>
{
    template<template <size_t Dim> class StencilCodeImpl,
        class GridReadViewType,
        class GridWriteViewType>
        static auto execute(GridReadViewType& source, GridWriteViewType& target)
    {
        using error_type = decltype(StencilCodeImpl<1>
            ::template execute(source, { 0 }));

        error_type error = 0;

        #pragma omp parallel for reduction(+: error)
        for (auto i = 1u; i < source.extents()[0] - 1; ++i)
        {
            const auto new_value = StencilCodeImpl<1>
                ::template execute(source, { i });

            error += std::abs(new_value - source.at({ i }));

            target.at({ i }) = new_value;
        }

        return error;
    }
};

template<>
struct iteration<2>
{
    template<template <size_t Dim> class StencilCodeImpl, 
        class GridReadViewType, 
        class GridWriteViewType>
    static auto execute(GridReadViewType& source, GridWriteViewType& target)
    {
        using error_type = decltype(StencilCodeImpl<2>
            ::template execute(source, { 0, 0 }));

        error_type error = 0;
        
        //#pragma omp parallel for reduction(+: error)
        for (auto y = 1u; y < source.extents()[1] - 1; ++y)
        {
            for (auto x = 1u; x < source.extents()[0] - 1; ++x)
            {
                const auto new_value = StencilCodeImpl<2>
                    ::template execute(source, { x, y });

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
    template<template <size_t Dim> class StencilCodeImpl,
        class GridReadViewType,
        class GridWriteViewType>
        static auto execute(GridReadViewType& source, GridWriteViewType& target)
    {
        using error_type = decltype(StencilCodeImpl<3>
            ::template execute(source, { 0, 0, 0 }));

        error_type error = 0;
        
        #pragma omp parallel for reduction(+: error)
        for (auto z = 1u; z < source.extents()[2] - 1; ++z)
        {
            for (auto y = 1u; y < source.extents()[1] - 1; ++y)
            {
                for (auto x = 1u; x < source.extents()[0] - 1; ++x)
                {
                    const auto new_value = StencilCodeImpl<3>
                        ::template execute(source, { x, y, z });

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
    static auto do_iteration(GridType& grid, const DiffType epsilon)
    {
        using iteration_type = iteration<get_dim<GridType>()>;
        using return_type = std::tuple<const int, const bool, const DiffType>;

        static constexpr auto batch_threshold = 1000000;
        static constexpr auto batch_size = 4;

        decltype(auto) first = get_first(grid);
        decltype(auto) second = get_second(grid);

        const auto iterations = 2 + [&]()
        {
            if (stencil_code::size(first.extents()) < batch_threshold)
            {
                for (auto i = 0u; i < batch_size; ++i)
                {
                    iteration_type::template execute<StencilCodeImpl>(first, second);
                    iteration_type::template execute<StencilCodeImpl>(second, first);
                }

                return 2 * batch_size;
            }

            return 0;
        }();
        
        iteration_type::template execute<StencilCodeImpl>(first, second);
        
        const auto error = iteration_type::template execute<StencilCodeImpl>(second, first);

        return return_type(iterations, error < epsilon, error);
    } 
};



}

#endif // ITERATION_H
