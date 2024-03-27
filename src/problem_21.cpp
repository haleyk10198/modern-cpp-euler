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
#include <vector>

using std::literals::string_view_literals::operator""sv;
using boost::multiprecision::cpp_int;
using namespace std::chrono;

std::generator<int> get_divisors(int x) {
    for (int i{1}; i <= x / i; ++i) {
        if (x % i == 0) {
            co_yield i;
            if (x / i != i) {
                co_yield x / i;
            }
        }
    }
}

int d(int x) {
    return std::ranges::fold_left(
            get_divisors(x) |
            std::views::filter([&x](auto&& dx) { return dx < x; }),
            0, std::plus<>());
}

int main() {
    auto nums = std::views::iota(1, 10000) | std::views::filter([](int x) {
        int dx = d(x);
        int ddx = d(dx);
        return ddx == x && dx != x;
    });
    std::cout << d(220) << ' ' << d(284) << '\n';
    std::cout << std::ranges::fold_left_first(nums, std::plus<>()).value()
              << '\n';
    return 0;
}
