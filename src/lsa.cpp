#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>


namespace lsa
{
    constexpr static auto Sxx{[](auto const &x) noexcept
    {
        return std::inner_product(std::cbegin(x), std::cend(x), std::cbegin(x), 0.0);
    }};

    constexpr static auto S1overx{[](auto const &x) noexcept
    {
        using A = typename std::remove_reference<decltype(x)>::type;
        using B = typename std::remove_const<A>::type;
        B aux(x.size());
        std::transform(std::cbegin(x), std::cend(x), std::begin(aux), [](auto const i) noexcept
        {
            return 1.0 / i; 
        });
        return Sxx(aux);
    }};

    constexpr static auto Sxy{[](auto const &x, auto const &y) noexcept
    {
        return std::inner_product(std::begin(x), std::end(x), std::begin(y), 0.0);
    }};

    constexpr static auto Syoverx{[](auto const &x, auto const &y) noexcept
    {
        using A = typename std::remove_reference<decltype(x)>::type;
        using B = typename std::remove_const<A>::type;
        B aux(x.size());
        std::transform(std::cbegin(x), std::cend(x), std::begin(aux), [](auto const i)
        {
            return 1.0 / i;
        });
        return Sxy(aux, y);
    }};

    constexpr static auto predict{[](auto const a, auto const x, auto const b) noexcept
    {
        return a*x + b/x;
    }};
} // namespace lsa

int main(int argc, char const *argv[])
{
    std::vector<double> const x({-1.0, 0.5, 1.0});
    std::vector<double> const y({-3.2, 4.55, 3.2});
    auto const n{x.size()};
    auto const sxx{lsa::Sxx(x)};
    auto const s1overx{lsa::S1overx(x)};
    auto const sxy{lsa::Sxy(x, y)};
    auto const syoverx{lsa::Syoverx(x, y)};
    auto const a{(n * syoverx - sxy * s1overx) / (n * n - sxx * s1overx)};
    auto const b{(sxy - a * sxx) / n};
    std::cout << "a : " <<  a << " b : " << b << std::endl;
    std::for_each(std::cbegin(x), std::cend(x),[&](auto const xi)
    {
        std::cout << "predicted y : " << lsa::predict(a, xi, b) << std::endl;
    });
    return 0;
}