#include <algorithm>
#include <any>
#include <cmath>
#include <concepts>
#include <functional>
#include <generator>
#include <iostream>
#include <mutex>
#include <ranges>
#include <unordered_set>
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

constexpr int ADJACENT_FACTOR = 4;

int main() {
    is_prime(2);
    auto packs =
            std::views::iota(100'000, 300'000) | std::views::transform([](int num) {
                return std::tuple{
                        num, (primes | std::views::take_while([num](int prime) {
                            return num >= prime;
                        }) |
                              std::views::filter(
                                      [num](int prime) { return num % prime == 0; }) |
                              std::views::take(ADJACENT_FACTOR) |
                              std::ranges::to<std::vector>())
                                .size()};
            }) |
            std::views::adjacent<ADJACENT_FACTOR>;
    auto [ans, _] = std::get<0>(*std::ranges::find_if(packs, [](auto&& pack) {
        return std::apply(
                [](auto&&... kvs) {
                    return ((std::get<1>(kvs) == ADJACENT_FACTOR) && ...);
                },
                pack);
    }));

    std::cout << ans << '\n';
}