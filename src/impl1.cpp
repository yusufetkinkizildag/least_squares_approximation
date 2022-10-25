#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>


namespace lsa
{
    constexpr static auto sum_of_mul{[](auto const &x, auto const &y) noexcept
    {
        return std::transform_reduce(std::cbegin(x), std::cend(x), std::cbegin(y), 0.0, std::plus{}, std::multiplies{});
    }};

    constexpr static auto  multiplicative_inverse{[](auto const i) noexcept { return 1.0 / i; }};

    constexpr static auto S1overx{[](auto const &x) noexcept
    {
        using A = typename std::remove_reference_t<decltype(x)>;
        using B = typename std::remove_const_t<A>;
        B aux(x.size());
        std::transform(std::cbegin(x), std::cend(x), std::begin(aux), multiplicative_inverse);
        return sum_of_mul(aux, aux);
    }};

    constexpr static auto Syoverx{[](auto const &x, auto const &y) noexcept
    {
        using A = typename std::remove_reference_t<decltype(x)>;
        using B = typename std::remove_const_t<A>;
        B aux(x.size());
        std::transform(std::cbegin(x), std::cend(x), std::begin(aux), multiplicative_inverse);
        return sum_of_mul(aux, y);
    }};

    constexpr static auto fit{[](auto const &x, auto const &y) noexcept
    {
        auto const n{x.size()};
        auto const sxx{lsa::sum_of_mul(x, x)};
        auto const s1overx{lsa::S1overx(x)};
        auto const sxy{lsa::sum_of_mul(x, y)};
        auto const syoverx{lsa::Syoverx(x, y)};
        auto const a{(n * syoverx - sxy * s1overx) / (n * n - sxx * s1overx)};
        auto const b{(sxy - a * sxx) / n};
        return std::make_pair(a, b);
    }};

    constexpr static auto predict{[](auto const &params, auto const x) noexcept
    {
        return params.first * x + params.second / x;
    }};

    constexpr static auto predict_vec{[](auto const &params, auto const &X) noexcept
    {
        using A = typename std::remove_reference_t<decltype(X)>;
        using B = typename std::remove_const_t<A>;
        B predictions(X.size());
        std::transform(std::cbegin(X), std::cend(X), std::begin(predictions), std::bind(predict, params, std::placeholders::_1));
        return predictions;
    }};
} // namespace lsa

int main(int argc, char const *argv[])
{
    std::vector<double> const X({-1.0, 0.5, 1.0});
    std::vector<double> const Y({-3.2, 4.55, 3.2});

    auto const params{lsa::fit(X, Y)};
    
    std::cout << "a : " <<  params.first << " b : " << params.second << std::endl;
    
    for (auto &&i : lsa::predict_vec(params, X))
    {
        std::cout << "predicted y : " << i << '\n';
    }

    return 0;
}
