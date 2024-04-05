#include <algorithm>
#include <complex>
#include <generator>
#include <iostream>
#include <ranges>
#include <unordered_set>
#include <vector>

constexpr auto primes = {2, 3, 5, 7, 11, 13, 17};

std::generator<std::vector<int> const &> get_pandigital() {
    auto digits = std::views::iota(0, 10) | std::ranges::to<std::vector>();
    do {
        co_yield digits;
    } while (std::ranges::next_permutation(digits).found);
}

constexpr auto example = {1, 4, 0, 6, 3, 5, 7, 2, 8, 9};

inline struct {
    template <typename R>
    constexpr bool operator()(R &&digits) {
        auto substrs = std::forward<decltype(digits)>(digits) |
                       std::views::adjacent<3> | std::views::drop(1) |
                       std::views::transform([](auto &&pack) {
                           auto [x, y, z] = pack;
                           return x * 100 + y * 10 + z;
                       });

        return std::ranges::all_of(std::views::zip(substrs, primes),
                                   [](auto &&pack) {
                                       auto [substr, prime] = pack;
                                       return substr % prime == 0;
                                   });
    }
} is_interesting;

inline struct {
    template <typename R>
    constexpr auto operator()(R &&digits) {
        return std::ranges::fold_left(
                std::forward<decltype(digits)>(digits), 0LL,
                [](auto acc, auto digit) {
                    return acc * 10 + digit;
                });
    }
} to_decimal;

int main() {
    static_assert(is_interesting(example));
    static_assert(to_decimal(example) == 1406357289LL);

    auto interesting_pandigital =
            get_pandigital() | std::views::filter(is_interesting) |
            std::views::transform(to_decimal) | std::ranges::to<std::vector>();

    std::cout << std::ranges::fold_left(interesting_pandigital, 0LL,
                                        std::plus<>())
              << '\n';
}