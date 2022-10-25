# least_squares_approximation

Given $\\{x,y\\}$ pairs, predict the $y$ in the form of $$y = ax + \frac{b}{x}$$ with the least squares approximation method.

The code in this project is a direct implementation of the formulas derived below. This [link](https://www.quora.com/How-do-you-solve-for-fitting-the-curve-y-ax-b-x-using-the-least-square-method) helped me about the derivation of the formulas.

## Requirements
gcc 9.4.0 

More info on [compiler](#compiler)

## How to compile
```console
root@docker-desktop:/# cd /path/to/least_squares_approximation
root@docker-desktop:/least_squares_approximation# bash clean_build.sh
```

## How to run
```console
root@docker-desktop:/# cd /path/to/least_squares_approximation
root@docker-desktop:/least_squares_approximation# ./build/impl1 # or ./build/impl2 or ./build/impl3 
```

## clean_build.sh
A trivial shell script that checks and deletes if `build` folder exists.
Calls the cmake and make to build and compile the source code

## CMakeLists.txt
A basic cmake file to specify C++ standard, build type and compile options

## Derivation of the Formulas

With $$i \in \mathbb{N}$$ 
and $$x_i, \\ y_i  \in \mathbb{R}$$
Let $$X = \\{x_1,\\ x_2, \\ \dots \\ x_i\\}$$
$$Y = \\{y_1,\\ y_2, \\ \dots \\ y_i\\}$$ so that  the error term of the ith element is defined as 
$$e_i = y_i - \left( ax_i + \frac{b}{x_i} \right) $$

When using the least squares approximation method, coefficients $a$ and $b$ are calculated by setting the partial derivatives of the $$E = \sum_{i=1}^{n}\left(y_i - \left( ax_i + \frac{b}{x_i}\right) \right)^2$$ to zero with respect to $a$ and $b$.


$$ \frac{\partial E}{\partial a} = \sum_{i=1}^{n}-2x_i \left(y_i - \left( ax_i + \frac{b}{x_i}\right)\right) = 0$$

$$ \boxed{\frac{\partial E}{\partial a} = \sum_{i=1}^{n} \left(-2x_iy_i + 2a{x_i}^2 + 2b\right) = 0}$$

$$ \frac{\partial E}{\partial b} = \sum_{i=1}^{n}-\frac{2}{x_i} \left(y_i - \left( ax_i + \frac{b}{x_i}\right)\right) = 0$$

$$ \boxed{\frac{\partial E}{\partial b} = \sum_{i=1}^{n} \left(-\frac{2y_i}{x_i} + 2a + \frac{2b}{{x_i}^2} \right) = 0}$$

$$ \frac{\partial E}{\partial a} = -\sum_{i=1}^{n} x_i y_i + \sum_{i=1}^{n}a{x_i}^2 + n b = 0$$

$$\frac{\partial E}{\partial b} = -\sum_{i=1}^{n}\frac{y_i}{x_i} + n a  + \sum_{i=1}^{n}\frac{b}{{x_i}^2} = 0$$

If rearranged, 

$$ b + \frac{1}{n} \sum_{i=1}^{n}a{x_i}^2 = \frac{1}{n} \sum_{i=1}^{n}{x_i y_i} $$

$$ a + \frac{1}{n} \sum_{i=1}^{n}\frac{b}{{x_i}^2} = \frac{1}{n} \sum_{i=1}^{n}\frac{y_i}{x_i} $$

If we represent b in terms of a and solve for a, a's estimator becomes

$$\hat a = \frac{n \sum{\frac{y_i}{x_i}} - \sum x_iy_i \sum{\frac{1}{{x_i}^2}} } {n^2 - \sum{{x_i}^2} \sum{\frac{1}{{x_i}^2}}}$$

the b's estimator becomes

$$\hat b = \frac{1}{n} \sum x_iy_i - \frac{\hat a}{n} \sum{{x_i}^2}$$

## impl1.cpp
This implementation uses lambdas to calculate the summations. The `lsa::fit` function takes two vector of double as the arguments and assumes that these two vectors has the same length. The terms in the formulas are directly named as variables. For example $\sum{{x_i}^2}$ is `auto const sxx{lsa::sum_of_mul(x, x)};`. The `sum_of_mul` stands for the 'summation of the multiplications' which uses [std::transform_reduce](https://en.cppreference.com/w/cpp/algorithm/transform_reduce) function to iterate over the given vectors. The lambda returns the accumulated result of the element wise multiplication of the two vectors. When the two of the vectors refers to the same vector, it means that dot product of that vector will be returned.

$\sum{\frac{1}{{x_i}^2}}$ is `auto const s1overx{lsa::S1overx(x)};`

$\sum{x_i y_i}$ is `auto const sxy{lsa::sum_of_mul(x, y)};`

$\sum{\frac{y_i}{x_i}}$ is `auto const syoverx{lsa::Syoverx(x, y)};`

`S1overx` is another lambda that finds the underlying data type of the given argument (which is a vector of double in this case) using [std::remove_reference](https://en.cppreference.com/w/cpp/types/remove_reference) and [std::remove_const](https://en.cppreference.com/w/cpp/types/remove_cv). Constructs an auxillary vector that holds the multiplicative inverse values of the given vector x and calls the `sum_of_mul` lambda with the two arguments being the same auxillary vector to calculate the term $\sum{\frac{1}{{x_i}^2}}$

The `Syoverx` is the same but calls the `sum_of_mul` lambda with the aux and the second vector (y).  

## impl2.cpp

## impl3.cpp

test

## Compiler
```console
root@docker-desktop:/generate_points_on_circle$ g++ -v
Using built-in specs.
COLLECT_GCC=g++
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/9/lto-wrapper
OFFLOAD_TARGET_NAMES=nvptx-none:hsa
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 9.4.0-1ubuntu1~20.04.1' --with-bugurl=file:///usr/share/doc/gcc-9/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++,gm2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-9 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-9-Av3uEd/gcc-9-9.4.0/debian/tmp-nvptx/usr,hsa --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.1)
```

```console
root@docker-desktop:/generate_points_on_circle$ gcc -v
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/9/lto-wrapper
OFFLOAD_TARGET_NAMES=nvptx-none:hsa
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 9.4.0-1ubuntu1~20.04.1' --with-bugurl=file:///usr/share/doc/gcc-9/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++,gm2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-9 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-9-Av3uEd/gcc-9-9.4.0/debian/tmp-nvptx/usr,hsa --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.1)
```
