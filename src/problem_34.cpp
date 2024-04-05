#include <algorithm>
#include <boost/functional/hash.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <charconv>
#include <chrono>
#include <generator>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <ranges>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <unordered_set>
#include <vector>

using std::literals::string_view_literals::operator""sv;
using boost::multiprecision::cpp_int;
using namespace std::chrono;

std::generator<int> digits(int x) {
    for (; x; x /= 10) {
        co_yield x % 10;
    }
}

inline struct {
    template <typename R>
    auto operator()(R&& range) {
        return std::ranges::fold_left(std::forward<R>(range),
                                      std::ranges::range_value_t<R>{},
                                      std::plus<>{});
    }
} range_sum;

std::generator<int> factorial_stream() {
    int acc = 1;
    for (int x = 1;; acc *= x, ++x) {
        co_yield acc;
    }
}

int factorial(int digit) {
    static auto factorial_v = factorial_stream() | std::views::take(10) |
                              std::ranges::to<std::vector>();
    return factorial_v[digit];
}

int main() {
    auto nums =
            std::views::iota(10, 1'000'000) | std::views::filter([](int num) {
                return num ==
                       range_sum(digits(num) | std::views::transform(factorial));
            });

    std::cout << range_sum(nums) << '\n';
}
