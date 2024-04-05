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

constexpr auto PRIME_CHECK_BOUND = 2'000'010;
bool sieve[PRIME_CHECK_BOUND];
void sieving() {
    sieve[0] = sieve[1] = 1;
    for (int i = 2; i < PRIME_CHECK_BOUND; ++i) {
        if (!sieve[i]) {
            for (int j = i * 2; j < PRIME_CHECK_BOUND; j += i) {
                sieve[j] = 1;
            }
        }
    }
}

bool is_prime(int num) {
    static std::once_flag f1;
    std::call_once(f1, sieving);

    if (num < 0) {
        return false;
    }

    return !sieve[num];
}

inline struct {
    template <std::integral T>
    std::generator<T> operator()(T x) const {
        T base{1};
        for (; base <= x; base *= 10) {
        }

        base /= 10;
        for (T deci{}; x; x %= base, base /= 10) {
            deci = x / base;
            co_yield x;
        }
    }
} truncate_left;

inline struct {
    template <std::integral T>
    std::generator<T> operator()(T x) const {
        for ( ; x; x /= 10) {
            co_yield x;
        }
    }
} truncate_right;

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
            std::views::iota(11, PRIME_CHECK_BOUND) | std::views::filter([](auto&& num) {
                return std::ranges::all_of(truncate_left(num), is_prime)
                       && std::ranges::all_of(truncate_right(num), is_prime);
            }) |
            std::views::take(11);

    std::ranges::for_each(nums, [](int num) { std::cout << num << '\n'; });

    std::cout << range_sum(nums) << '\n';
}
