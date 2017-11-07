#ifndef PI_H
#define PI_H

#include <random>
#include <tuple>
#include <chrono>
#include <thread>
#include <omp.h>  

namespace pi
{
    using value_t = double;
    using point_t = std::tuple<value_t, value_t>;

    namespace detail
    {
        namespace seq_1
        {
            template<class RandomEngineType, class RandomDistributionType>
            point_t generate_point(RandomEngineType& rnd, RandomDistributionType& rng)
            {
                return { rng(rnd), rng(rnd) };
            }

            inline double do_calculate(const uint64_t samples)
            {
                std::default_random_engine rnd;
                std::uniform_real_distribution<double> rng(-1, 1);

                auto num_inside = 0ull;

                for (auto i = 0ull; i < samples; i++)
                {
                    auto[x, y] = generate_point(rnd, rng);

                    if (x * x + y * y > 1) continue;

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
                return { rng(rnd), rng(rnd) };
            }

            inline double do_calculate(const uint64_t samples)
            {
                std::default_random_engine rnd;
                std::uniform_real_distribution<double> rng(-1, 1);

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

        namespace par
        {
            template<class RandomEngineType, class RandomDistributionType>
            point_t generate_point(RandomEngineType& rnd, RandomDistributionType& rng)
            {
                return { rng(rnd), rng(rnd) };
            }

            inline double do_calculate(const uint64_t samples)
            {
                auto num_inside = 0ui64;

                if (samples > static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
                    throw std::runtime_error("argument out of range");

                #pragma omp parallel
                {
                    thread_local std::default_random_engine rnd;
                    thread_local std::uniform_real_distribution<double> rng(-1, 1);
            
                    rnd.seed(omp_get_thread_num());

                    #pragma omp for reduction(+: num_inside)
                    for (auto i = 0i64; i < static_cast<int64_t>(samples); i++)
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
                }

                return 4 * num_inside / static_cast<value_t>(samples);
            }
        }
    }

    enum method
    {
        seq,
        seq_optimized,
        parallel
    };

    template<method Method>
    double calculate(const uint64_t samples)
    {
        if constexpr(Method == seq)
        {
            return detail::seq_1::do_calculate(samples);
        }
        else if constexpr(Method == seq_optimized)
        {
            return detail::seq_2::do_calculate(samples);
        }
        else if constexpr(Method == parallel)
        {
            return detail::par::do_calculate(samples);
        }
        else
        {
            throw std::exception("not supported");
        }
    }
}





#endif // PI_H
