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

                    ++num_inside;
                }

                return 4 * num_inside / static_cast<value_t>(samples);
            }
        }

        namespace seq_2
        {
            template<class RandomEngineType, class RandomDistributionType>
            point_t generate_point(RandomEngineType& rnd, RandomDistributionType& rng)
            {
                return point_t(rng(rnd), rng(rnd));
            }

            inline value_t do_calculate(const uint64_t samples)
            {
                std::default_random_engine rnd;
                std::uniform_real_distribution<value_t> rng(-1, 1);

                auto num_inside = 0ull;

                for (auto i = 0ull; i < samples; i++)
                {
                    //const auto [x, y] = generate_point(rnd, rng);
                    const auto point = generate_point(rnd, rng);

                    const auto x = std::get<0>(point);
                    const auto y = std::get<1>(point);

                    // 0 <= dist <= 2 
                    const auto dist = static_cast<int>(x * x + y * y);

                    // is_two == 1 iff dist == 2
                    const auto is_two = dist / 2;

                    // inside:
                    // (1 - dist) == 1  
                    // (1 - is_two) == 1 
                    //
                    // outside:
                    // (1 - dist) == 0  
                    // (1 - is_two) == 1 
                    //
                    // on edge:
                    // (1 - dist) == -1  
                    // (1 - is_two) == 0 
                    num_inside += (1 - dist) * (1 - is_two);
                }

                return 4 * num_inside / static_cast<value_t>(samples);
            }
        }

        namespace seq_3
        {
            inline value_t do_calculate(const uint64_t samples)
            {
                std::default_random_engine rnd_x;
                std::uniform_real_distribution<value_t> rng_x(-1, 1);

                std::default_random_engine rnd_y;
                std::uniform_real_distribution<value_t> rng_y(-1, 1);

                rnd_x.seed(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
                rnd_y.seed(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

                std::vector<value_t> xs;
                std::vector<value_t> ys;

                xs.reserve(samples);
                ys.reserve(samples);

                #ifndef _MSC_VER 
                #pragma omp simd
                #endif
                for (auto i = 0ull; i < samples; i++)
                {
                    xs[i] = rng_x(rnd_x);
                    ys[i] = rng_y(rnd_y);
                }

                constexpr auto num_buffers = 8;
                std::array<std::array<unsigned long long, 3>, num_buffers> nums_inside;

                #ifndef _MSC_VER 
                #pragma omp simd
                #endif
                for (auto i = 0ull; i < samples; i++)
                {
                    const auto j = static_cast<int>(xs[i] * xs[i] + ys[i] * ys[i]);

                    ++nums_inside[i % num_buffers][j];
                }

                auto num_inside = 0ull;
                for (const auto& n : nums_inside)
                {
                    num_inside += n[0];
                }

                return num_inside * 4 / static_cast<value_t>(samples);
            }
        }

        namespace par
        {
            template<class RandomEngineType, class RandomDistributionType>
            point_t generate_point(RandomEngineType& rnd, RandomDistributionType& rng)
            {
                return { rng(rnd), rng(rnd) };
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

                        if(x * x + y * y >= 1) continue;

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
                if (samples > static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
                    throw std::runtime_error("argument out of range");

                auto num_inside = 0ull;

                #pragma omp parallel reduction(+: num_inside)
                {
                    // use two independend random number devices and distributions
                    // to reduce dependencies
                    thread_local std::default_random_engine rnd_x;
                    thread_local std::uniform_real_distribution<value_t> rng_x(-1, 1);

                    thread_local std::default_random_engine rnd_y;
                    thread_local std::uniform_real_distribution<value_t> rng_y(-1, 1);

                    rnd_x.seed(omp_get_thread_num());
                    rnd_y.seed(2 * omp_get_num_threads() - omp_get_thread_num());
                    
                    std::vector<value_t> xs;
                    std::vector<value_t> ys;

                    // use reserve() to allocate memory without
                    // initializing the values
                    xs.reserve(samples);
                    ys.reserve(samples);

                    // precompute points and put the into a
                    // structure of arrays
                    #ifndef _MSC_VER 
                    #pragma omp for simd
                    #else
                    #pragma omp for
                    #endif
                    for (auto i = 0ll; i < static_cast<int64_t>(samples); i++)
                    {
                        xs[i] = rng_x(rnd_x);
                        ys[i] = rng_y(rnd_y);
                    }

                    // use multiple buffers to reduce direct data dependencies
                    constexpr auto num_buffers = 8;
                    using buffer_t = std::array<unsigned long long, num_buffers>;
                                        
                    // use array of buffers to remove branching by using
                    // computed value as array index
                    thread_local std::array<buffer_t, 3> buffers;

                    #ifndef _MSC_VER 
                    #pragma omp for simd
                    #else
                    #pragma omp for
                    #endif
                    for (auto i = 0ll; i < static_cast<int64_t>(samples); i++)
                    {
                        // j == 0 iff current point inside the circle
                        // j == 1 iff current point outside the circle
                        // j == 2 iff current point on the edge of the rectangle
                        const auto j = static_cast<int>(xs[i] * xs[i] + ys[i] * ys[i]);

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
    }

    enum method
    {
        seq,
        seq_2,
        seq_3,
        par,
        par_2
    };

    template<method Method>
    double calculate(const uint64_t samples)
    {
        if (Method == seq)
        {
            return detail::seq_1::do_calculate(samples);
        }
        if (Method == seq_2)
        {
            return detail::seq_2::do_calculate(samples);
        }
        if (Method == seq_3)
        {
            return detail::seq_3::do_calculate(samples);
        }
        if (Method == par)
        {
            return detail::par::do_calculate(samples);
        }
        if (Method == par_2)
        {
            return detail::par_2::do_calculate(samples);
        }

        throw std::runtime_error("not supported");
    }
}





#endif // PI_H
