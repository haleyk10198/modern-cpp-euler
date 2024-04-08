#include <algorithm>
#include <any>
#include <cmath>
#include <concepts>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <ranges>
#include <unordered_set>
#include <utility>
#include <vector>

constexpr auto PRIME_CHECK_BOUND = 1'000'010;
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

int main() {
    is_prime(2);

}