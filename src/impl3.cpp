#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

namespace lsa
{
    struct Summations
    {
        double x_squared;
        double one_over_x_squared;
        double xy;
        double y_over_x;

        Summations& operator+(Summations const &rhs)
        {
            this->x_squared += rhs.x_squared;
            this->one_over_x_squared += rhs.one_over_x_squared;
            this->xy += rhs.xy;
            this->y_over_x += rhs.y_over_x;
            return *this;
        }

        friend Summations operator+(Summations const &lhs, Summations const &rhs)
        {
            return Summations{.x_squared{lhs.x_squared + rhs.x_squared},
                              .one_over_x_squared{lhs.one_over_x_squared + rhs.one_over_x_squared},
                              .xy{lhs.xy + rhs.xy},
                              .y_over_x{lhs.y_over_x + rhs.y_over_x}};
        }
    };

    class LSA
    {
        std::pair<double, double> params;

    public:
        std::pair<double, double> get_params() const noexcept
        {
            return this->params;
        }

        double predict(double const x) const noexcept
        {
            return this->params.first * x + this->params.second / x;
        }

        std::vector<double> predict(std::vector<double> const &X) const noexcept
        {
            std::vector<double> results(X.size());
            std::transform(std::cbegin(X), std::cend(X), std::begin(results), [this](auto const x) noexcept { return this->predict(x); });
            return results;
        }

        std::pair<double, double> fit(std::vector<double> const &X, std::vector<double> const &Y)
        {
            auto const n{X.size()};
            Summations summations{std::transform_reduce(std::cbegin(X), std::cend(X), std::cbegin(Y), Summations{}, std::plus{},
            [](auto const x, auto const y) noexcept
            {
                return Summations{.x_squared{std::pow(x, 2)}, .one_over_x_squared{std::pow(1 / x, 2)}, .xy{x * y}, .y_over_x{y / x}};
            })};
            this->params.first = (n * summations.y_over_x - summations.xy * summations.one_over_x_squared) / (std::pow(n, 2) - summations.x_squared * summations.one_over_x_squared);
            this->params.second = (summations.xy - this->params.first * summations.x_squared) / n;
            return this->params;
        }

        LSA(std::vector<double> const &X, std::vector<double> const &Y) noexcept
        {
            this->fit(X, Y);
        }

        LSA() = default;
    };

} // namespace lsa

int main(int argc, char const *argv[])
{
    std::vector<double> const X({-1.0, 0.5, 1.0});
    std::vector<double> const Y({-3.2, 4.55, 3.2});

    lsa::LSA obj1{X, Y};

    auto const params{obj1.get_params()};
    std::cout << "a : " << params.first << " b : " << params.second << std::endl;

    for (auto &&i : obj1.predict(X))
    {
        std::cout << "predicted y : " << i << '\n';
    }



    lsa::LSA obj2{};

    auto const params2{obj2.get_params()};
    std::cout << "a : " << params2.first << " b : " << params2.second << std::endl;

    for (auto &&i : obj2.predict(X))
    {
        std::cout << "predicted y : " << i << '\n';
    }

    obj2.fit(X, Y);

    auto const params3{obj2.get_params()};
    std::cout << "a : " << params3.first << " b : " << params3.second << std::endl;

    for (auto &&i : obj2.predict(X))
    {
        std::cout << "predicted y : " << i << '\n';
    }


    return 0;
}
