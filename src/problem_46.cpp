#include <algorithm>
#include <any>
#include <boost/any.hpp>
#include <cmath>
#include <concepts>
#include <functional>
#include <generator>
#include <iostream>
#include <mutex>
#include <ranges>
#include <utility>
#include <vector>

constexpr auto PRIME_CHECK_BOUND = 2'000'010;
bool sieve[PRIME_CHECK_BOUND];

std::vector<int> primes;
void sieving() {
    sieve[0] = sieve[1] = 1;
    for (int i = 2; i < PRIME_CHECK_BOUND; ++i) {
        if (!sieve[i]) {
            primes.push_back(i);
            for (int j = i * 2; j < PRIME_CHECK_BOUND; j += i) {
                sieve[j] = 1;
            }
        }
    }
}

bool is_prime(int num) {
    static std::once_flag f1;
    std::call_once(f1, sieving);

    if (num < 0 || num >= PRIME_CHECK_BOUND) {
        return false;
    }

    return !sieve[num];
}

constexpr bool is_square(int num) {
    if (num < 0) {
        return false;
    }

    int sq = std::sqrt(num);
    return sq * sq == num;
}

int main() {
    is_prime(2);
    static_assert(is_square(1));
    auto witnesses =
            std::views::iota(2, 100'000) | std::views::filter([](int num) {
                return !is_prime(num) && num % 2 == 1;
            }) |
            std::views::filter([](int odd) {
                return !std::ranges::any_of(
                        primes | std::views::take_while(
                                [&odd](int prime) { return prime < odd; }),
                        [&odd](int prime) { return is_square((odd - prime) / 2); });
            }) |
            std::views::take(1) | std::ranges::to<std::vector>();

    std::cout << witnesses.front() << '\n';
}