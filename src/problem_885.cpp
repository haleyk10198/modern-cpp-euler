#include <algorithm>
#include <concepts>
#include <iostream>
#include <ranges>
#include <utility>

template <std::integral I>
struct modulated;

template <>
struct modulated<std::size_t> {
    using I = std::size_t;
    I value_{};
    static constexpr I MOD{1123455689uz};

    constexpr explicit modulated(std::integral auto value) : value_(value) {}

    // NOLINT
    constexpr operator I() const { return value_; }

    constexpr I operator*() const { return value_; }

    constexpr auto operator+(auto &&rhs) const {
        auto temp = *this;
        (temp.value_ += static_cast<I &&>(rhs) % MOD) %= MOD;
        return temp;
    }

    constexpr auto operator*(auto &&rhs) const {
        auto temp = *this;
        (temp.value_ *= static_cast<I &&>(std::forward<decltype(rhs)>(rhs))) %=
                MOD;
        return temp;
    }

    constexpr auto &operator*=(auto &&rhs) {
        (value_ *= static_cast<I &&>(rhs)) %= MOD;
        return *this;
    }
};

using ans_t = modulated<std::size_t>;

template <std::size_t N>
constexpr ans_t digit = digit<N - 1> * 10;

template <>
constexpr ans_t digit<0>{1uz};

template <std::size_t FROM, std::size_t TO>
constexpr ans_t digit_sum =
        []<std::size_t... Is>(std::index_sequence<Is...>) consteval {
            return (digit<FROM + Is> + ...);
        }(std::make_index_sequence<TO - FROM>());

template <std::size_t FROM>
constexpr ans_t digit_sum<FROM, FROM>{0};

constexpr auto pow(ans_t base, std::size_t exp) {
    ans_t result{1};
    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

constexpr auto pow(std::integral auto base, std::size_t exp) {
    return pow(ans_t{base}, exp);
}

constexpr ans_t factorial(std::size_t n) {
    ans_t result{1};
    for (unsigned int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

constexpr ans_t mod_inverse(std::size_t n) {
    return pow(ans_t{n}, ans_t::MOD - 2);
}

constexpr ans_t nck(std::size_t n, std::size_t k) {
    return factorial(n) * mod_inverse(factorial(k)) *
           mod_inverse(factorial(n - k));
}

template <std::size_t N, int D = 0>
constexpr ans_t dpf =
        []<std::size_t... Is>(std::index_sequence<Is...>) consteval {
            return ((nck(N, Is) * (digit_sum<N - Is, N> * D * pow(9 - D, N - Is) +
                                   dpf<N - Is, D + 1>)) +
                    ...);
        }(std::make_index_sequence<N + 1>());

template <std::size_t N>
constexpr ans_t dpf<N, 9> = digit_sum<0, N> * 9;

int main() {
    static_assert(digit_sum<0, 2> == 11);
    static_assert(nck(4, 2) == 6);
    static_assert(factorial(4) == 24);
    static_assert(mod_inverse(4) * ans_t{4} == 1);
    static_assert(dpf<1> == 45);
    static_assert(dpf<1, 9> == 9);
    static_assert(dpf<1, 8> == 8 + 9);
    static_assert(dpf<1, 7> == 7 + 8 + 9);
    static_assert(dpf<5> == 1543545675 % ans_t::MOD);
    static_assert(dpf<2> == 3465);
    static_assert(dpf<3> == 254475);
    static_assert(dpf<4> == 19457757);
    static_assert(dpf<2, 9> == 99);
    static_assert(dpf<2, 8> == 99 + 89 * 2 + 88);
    static_assert(dpf<2, 7> == 99 + 88 + 77 + (78 + 79 + 89) * 2);
    std::cout << dpf<40> << std::endl;

    return 0;
}
