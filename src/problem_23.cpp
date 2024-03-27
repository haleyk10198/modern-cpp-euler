#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>
#include <charconv>
#include <chrono>
#include <generator>
#include <iostream>
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

std::generator<int> get_proper_divisors(int x) {
    if (x <= 1) {
        co_return;
    }
    co_yield (1);
    for (int i{2}; i <= x / i; ++i) {
        if (x % i == 0) {
            co_yield i;
            if (x / i != i) {
                co_yield x / i;
            }
        }
    }
}

int d(int x) {
    return std::ranges::fold_left(get_proper_divisors(x), 0, std::plus<>());
}

int main() {
    auto nums = std::views::iota(1, 28123) | std::views::filter([](int x) {
        int dx = d(x);
        return dx > x;
    }) |
                std::ranges::to<std::unordered_set>();

    auto ans =
            std::views::iota(1, 28123) | std::views::filter([&nums](auto&& x) {
                return std::ranges::none_of(
                        nums, [&nums, &x](auto&& yy) { return nums.count(x - yy); });
            });

    std::cout << std::ranges::fold_left_first(ans, std::plus<>()).value()
              << '\n';
    return 0;
}
