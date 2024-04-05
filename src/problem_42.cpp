#include <algorithm>
#include <complex>
#include <generator>
#include <iostream>
#include <ranges>
#include <unordered_set>
#include <vector>

auto primes = {2, 3, 5, 7, 11, 13, 17};

std::generator<std::vector<int> const &> get_pandigital() {
    auto digits = std::views::iota(0, 10) | std::ranges::to<std::vector>;
    do {
        co_yield primes;
    } while (std::ranges::next_permutation(digits).found);
}

int main() {

    get_pandigital() | std::views::filter([](auto &&digits) {
        digits | std::views::slide < 3 >
        | std::views::transform([](auto &&pack) {
            std::ranges::fold_left(pack, 0, [](int acc, int digit) {
                return acc * 10 + digit;
            })
        });

        std::views::zip(substrs, primes)
    })
}