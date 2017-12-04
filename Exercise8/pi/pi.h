#ifndef PI_H
#define PI_H

#include <random>
#include <tuple>
#include <chrono>
#include <thread>
#include <omp.h>  
#include <array>
#include <numeric>

namespace pi
{
    using value_t = float;
    using point_t = std::tuple<value_t, value_t>;

    namespace detail
    {
        namespace seq_1
        {
            template<class RandomEngineType, class RandomDistributionType>
            point_t generate_point(RandomEngineType& rnd, RandomDistributionType& rng)
            {
                return point_t( rng(rnd), rng(rnd) );
            }

            inline value_t do_calculate(const uint64_t samples)
            {
                std::default_random_engine rnd;
                std::uniform_real_distribution<value_t> rng(-1, 1);

                auto num_inside = 0ull;

                for (auto i = 0ull; i < samples; i++)
                {
                    const auto point = generate_point(rnd, rng);

                    const auto x = std::get<0>(point);
                    const auto y = std::get<1>(point);

                    if(x * x + y * y <= 1) ++num_inside;
                }

                return 4 * num_inside / static_cast<value_t>(samples);
            }
        }

        namespace seq_3
        {
            inline value_t do_calculate(const uint64_t samples)
            {
                // use two independend random number devices and distributions
                // to reduce dependencies
                std::default_random_engine rnd_x;
                std::uniform_real_distribution<value_t> rng_x(-1, 1);

                std::default_random_engine rnd_y;
                std::uniform_real_distribution<value_t> rng_y(-1, 1);

                rnd_x.seed(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
                rnd_y.seed(8u * static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

                constexpr auto num_buffers = 8;
                using buffer_t = std::array<unsigned long long, num_buffers>;

                // use array of buffers to remove branching by using
                // computed value as array index
                thread_local std::array<buffer_t, 3> buffers;

                #pragma omp simd
                for (auto i = 0ll; i < static_cast<int64_t>(samples); i++)
                {
                    const auto x = rng_x(rnd_x);
                    const auto y = rng_y(rnd_y);

                    // j == 0 iff current point inside the circle
                    // j == 1 iff current point outside the circle
                    // j == 2 iff current point on the edge of the rectangle
                    constexpr auto epsilon = std::numeric_limits<value_t>::min();
                    const auto j = static_cast<int>(x * x + y * y - epsilon);

                    ++buffers[j][i % num_buffers];
                }

                // only count points for which the squared distance
                // was below 1 ( => j == 0 )
                const auto& nums_inside = buffers[0];

                // accumulate values from all buffers
                const auto num_inside = std::accumulate(
                    nums_inside.begin(),
                    nums_inside.end(),
                    0ull);

                return 4 * num_inside / static_cast<value_t>(samples);
            }
        }

        namespace par
        {
            template<class RandomEngineType, class RandomDistributionType>
            point_t generate_point(RandomEngineType& rnd, RandomDistributionType& rng)
            {
                return point_t(rng(rnd), rng(rnd));
            }

            inline value_t do_calculate(const uint64_t samples)
            {
                auto num_inside = 0ull;

                if (samples > static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
                    throw std::runtime_error("argument out of range");

                #pragma omp parallel
                {
                    thread_local std::default_random_engine rnd;
                    thread_local std::uniform_real_distribution<value_t> rng(-1, 1);
            
                    rnd.seed(omp_get_thread_num());

                    #pragma omp for reduction(+: num_inside)
                    for (auto i = 0ll; i < static_cast<int64_t>(samples); i++)
                    {
                        //const auto [x, y] = generate_point(rnd, rng);
                        const auto point = generate_point(rnd, rng);

                        const auto x = std::get<0>(point);
                        const auto y = std::get<1>(point);

                        constexpr auto epsilon = std::numeric_limits<value_t>::min();

                        if(static_cast<int>(x * x + y * y - epsilon) == 0) 
                            ++num_inside;
                    }  
                }

                return 4 * num_inside / static_cast<value_t>(samples);
            }
        }

        namespace par_2
        {
            inline value_t do_calculate(const uint64_t samples)
            {
                //if (samples > static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
                //    throw std::runtime_error("argument out of range");

                auto num_inside = 0ull;

                #pragma omp parallel reduction(+: num_inside)
                {
                    // use two independend random number devices and distributions
                    // to reduce dependencies
                    thread_local std::default_random_engine rnd_x;
                    thread_local std::uniform_real_distribution<value_t> rng_x(-1, 1);

                    thread_local std::default_random_engine rnd_y;
                    thread_local std::uniform_real_distribution<value_t> rng_y(-1, 1);

                    rnd_x.seed(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()) * omp_get_thread_num() + 1);
                    rnd_y.seed(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()) * ((2 * omp_get_num_threads() + 1) - omp_get_thread_num()));
                    
                    constexpr auto num_buffers = 8;
                    using buffer_t = std::array<unsigned long long, num_buffers>;
                                        
                    // use array of buffers to remove branching by using
                    // computed value as array index
                    thread_local std::array<buffer_t, 3> buffers;

                    #pragma omp for simd
                    for (auto i = 0ll; i < static_cast<int64_t>(samples); i++)
                    {
                        const auto x = rng_x(rnd_x);
                        const auto y = rng_y(rnd_y);

                        // j == 0 iff current point inside the circle
                        // j == 1 iff current point outside the circle
                        // j == 2 iff current point on the edge of the rectangle
                        constexpr auto epsilon = std::numeric_limits<value_t>::min();
                        const auto j = static_cast<int>(x * x + y * y - epsilon);

                        ++buffers[j][i % num_buffers];
                    }

                    // only count points for which the squared distance
                    // was below 1 ( => j == 0)
                    const auto& nums_inside = buffers[0];

                    // accumulate values from all buffers
                    num_inside = std::accumulate(
                        nums_inside.begin(), 
                        nums_inside.end(), 
                        num_inside);
                }

                return 4 * num_inside / static_cast<value_t>(samples);
            }
        }

        namespace mpi
        {
            inline size_t do_calculate(const uint64_t samples)
            {
                //if (samples > static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
                //    throw std::runtime_error("argument out of range");

                auto num_inside = 0ull;

                #pragma omp parallel reduction(+: num_inside)
                {
                    // use two independend random number devices and distributions
                    // to reduce dependencies
                    thread_local std::default_random_engine rnd_x;
                    thread_local std::uniform_real_distribution<value_t> rng_x(-1, 1);

                    thread_local std::default_random_engine rnd_y;
                    thread_local std::uniform_real_distribution<value_t> rng_y(-1, 1);

                    rnd_x.seed(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()) * omp_get_thread_num() + 1);
                    rnd_y.seed(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()) * ((2 * omp_get_num_threads() + 1) - omp_get_thread_num()));

                    constexpr auto num_buffers = 8;
                    using buffer_t = std::array<unsigned long long, num_buffers>;

                    // use array of buffers to remove branching by using
                    // computed value as array index
                    thread_local std::array<buffer_t, 3> buffers;

                    #pragma omp for simd
                    for (auto i = 0ll; i < static_cast<int64_t>(samples); i++)
                    {
                        const auto x = rng_x(rnd_x);
                        const auto y = rng_y(rnd_y);

                        // j == 0 iff current point inside the circle
                        // j == 1 iff current point outside the circle
                        // j == 2 iff current point on the edge of the rectangle
                        constexpr auto epsilon = std::numeric_limits<value_t>::min();
                        const auto j = static_cast<int>(x * x + y * y - epsilon);

                        ++buffers[j][i % num_buffers];
                    }

                    // only count points for which the squared distance
                    // was below 1 ( => j == 0)
                    const auto &nums_inside = buffers[0];

                    // accumulate values from all buffers
                    num_inside = std::accumulate(
                        nums_inside.begin(),
                        nums_inside.end(),
                        num_inside);
                }

                return num_inside;
            }
        }
    }

    enum method
    {
        seq,
        seq_3,
        par,
        par_2,
    };

    template<method Method>
    value_t calculate(const uint64_t samples)
    {
        if (Method == seq)
        {
            return detail::seq_1::do_calculate(samples);
        }

        if (Method == par)
        {
            return detail::par::do_calculate(samples);
        }

        if (Method == par_2)
        {
            return detail::par_2::do_calculate(samples);
        }

        if (Method == seq_3)
        {
            return detail::seq_3::do_calculate(samples);
        }

        throw std::runtime_error("not supported");
    }
}

#endif // PI_H
