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
                [](auto acc, auto digit) { return acc * 10 + digit; });
    }
} to_decimal;

using penta_t = long long;
penta_t to_pentagonal(int num) { return num * (3 * num - 1) / 2; }

template <std::integral T = penta_t>
struct min_num {
    T _num{std::numeric_limits<T>::max()};

    operator T() { return _num; }

    template <std::integral I>
    T &operator=(I num) {
        _num = std::min(_num, (T)num);
        return _num;
    }
};

int main() {
    min_num res;
    std::unordered_set<penta_t> pentas;
    for (penta_t n = 1, penta = to_pentagonal(1), prev = to_pentagonal(0);
         3 * n - 2 < res && n < 10'000; ++n, prev = penta, penta += 3 * n - 2) {
        for (auto pp : pentas) {
            auto pk = (penta + pp) / 2;
            auto pj = (penta - pp) / 2;
            if (pentas.count(pk) && pentas.count(pj)) {
                std::cout << pj << ' ' << pk << '\n';
                res = pp;
            }
        }
        pentas.insert(penta);
    }

    std::cout << res << '\n';
}