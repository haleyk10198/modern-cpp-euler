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

int pad_right(int& sum, int pad) { return sum = sum * 10 + pad; }

template <std::ranges::forward_range R>
int to_num(R&& range) {
    return std::ranges::fold_left(
            range, 0, [](int acc, int pad) { return pad_right(acc, pad); });
}

inline struct {
    template <typename R>
    auto operator()(R&& range) {
        return std::ranges::fold_left(range, std::ranges::range_value_t<R>{},
                                      std::plus<>());
    }
} range_sum;

std::generator<std::tuple<int, int, int>> pandigital_base() {
    auto digits = std::views::iota(1, 10) | std::ranges::to<std::vector>();

    do {
        for (int sep1 = 1, d1 = 0; sep1 <= 4; ++sep1) {
            pad_right(d1, digits[sep1 - 1]);
            for (int sep2 = sep1 + 1, d2 = 0; sep2 <= 5; ++sep2) {
                pad_right(d2, digits[sep2 - 1]);
                int d3 = to_num(
                        std::ranges::subrange(digits.begin() + sep2, digits.end()));

                co_yield {d1, d2, d3};
            }
        }
    } while (std::ranges::next_permutation(digits).found);
}

int main() {
    auto sums = pandigital_base() | std::views::filter([](auto&& pack) {
        auto [d1, d2, d3] = pack;
        if (d1 * d2 == d3) {
            std::cout << d3 << '\n';
        }
        return d1 * d2 == d3;
    }) |
                std::views::elements<2> | std::ranges::to<std::vector>() |
                std::ranges::to<std::unordered_set>();

    static_assert(
            std::same_as<int, std::ranges::range_value_t<decltype(sums)>>);
    auto ans = range_sum(sums);

    std::cout << ans << '\n';

    return 0;
}
