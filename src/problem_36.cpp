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

std::generator<int> to_base(int x, int base) {
    for (; x; x /= base) {
        co_yield x % base;
    }
}

bool is_palindrome(int num, int base) {
    auto based = to_base(num, base) | std::ranges::to<std::vector>();
    return based ==
           (based | std::views::reverse | std::ranges::to<std::vector>());
}

inline struct {
    template <typename R>
    auto operator()(R&& range) {
        return std::ranges::fold_left(std::forward<R>(range),
                                      std::ranges::range_value_t<R>{},
                                      std::plus<>{});
    }
} range_sum;

int main() {
    auto nums =
            std::views::iota(1, 1'000'000) | std::views::filter([](auto&& num) {
                return is_palindrome(num, 2) && is_palindrome(num, 10);
            });

    std::ranges::for_each(nums, [](int num) { std::cout << num << '\n'; });

    std::cout << range_sum(nums) << '\n';
}
