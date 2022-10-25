#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

namespace lsa
{
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
            std::transform(std::cbegin(X), std::cend(X), std::begin(results), [this](auto const x) noexcept
            {
                return this->predict(x);
            });
            return results;
        }

        LSA(std::vector<double> const &X, std::vector<double> const &Y) noexcept
        {
            auto const n{X.size()};
            auto sxx{0.0};
            auto s1overx{0.0};
            auto sxy{0.0};
            auto syoverx{0.0};
            for (auto i{0}; i != n; ++i)
            {
                auto const &x{X.at(i)};
                auto const &y{Y.at(i)};
                sxx += std::pow(x, 2);
                s1overx += std::pow(1/x, 2);
                sxy += x*y;
                syoverx += y/x;
            }
            this->params.first = (n * syoverx - sxy * s1overx) / (n * n - sxx * s1overx);
            this->params.second = (sxy - this->params.first * sxx) / n;
        }
    };

 
} // namespace lsa

int main(int argc, char const *argv[])
{
    std::vector<double> const X({-1.0, 0.5, 1.0});
    std::vector<double> const Y({-3.2, 4.55, 3.2});
    
    lsa::LSA obj{X, Y};
    
    auto const params{obj.get_params()};

    std::cout << "a : " << params.first << " b : " << params.second << std::endl;

    for (auto &&i : obj.predict(X))
    {
        std::cout << "predicted y : " << i << '\n';
    }

    return 0;
}
