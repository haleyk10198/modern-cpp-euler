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

inline struct {
    template <std::integral T>
    std::generator<T> operator()(T num) {
        while (num) {
            co_yield num % 10;
            num /= 10;
        }
    }
} to_digits;

inline struct {
    template <std::ranges::input_range R>
    auto operator()(R&& range) {
        return std::ranges::fold_left(range, std::ranges::range_value_t<R>{},
                                      std::plus<>());
    }
} range_sum;

int main() {
    auto nums =
            std::views::iota(10, 1e6) | std::views::filter([](int num) {
                return num == range_sum(to_digits(num) |
                                        std::views::transform([](int digit) {
                                            return std::pow(digit, 5);
                                        }));
            });

    std::ranges::for_each(nums, [](auto&& num) { std::cout << num << '\n'; });

    std::cout << range_sum(nums) << '\n';

    return 0;
}
