#include <algorithm>
#include <complex>
#include <generator>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <vector>

std::generator<int> to_digits(int num) {
    for (; num; num /= 10) {
        co_yield num % 10;
    }
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
            co_yield deci;
        }
    }
} to_digit;

int get_d(int n) {
    --n;
    for (int num_length = 1, base = 1;; base *= 10, ++num_length) {
        int coverage = base * 9 * num_length;
        if (n < coverage) {
            int number = base + n / num_length;
            n %= num_length;

            return *(to_digit(number) | std::views::drop(n - 1)).begin();
        }
        n -= coverage;
    }
}

std::generator<int> powers_of(int base) {
    for (int acc = 1, i = 0;; acc *= base) {
        co_yield acc;
    }
}

int main() {
    auto ds =
            powers_of(10) | std::views::take(7) | std::views::transform(get_d);

    std::ranges::for_each(ds, [](int d) { std::cout << d << '\n'; });

    // std::cout << std::ranges::fold_left(ds, 1, std::multiplies<>()) << '\n';
}