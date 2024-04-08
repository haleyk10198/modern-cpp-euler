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

int hash_prime(int prime) {
    std::vector<int> digits;
    for (; prime; prime /= 10) {
        digits.push_back(prime % 10);
    }

    std::ranges::sort(digits);

    return std::ranges::fold_left(
            digits, 0, [](int acc, int digit) { return acc * 10 + digit; });
}

template <std::size_t N, std::ranges::forward_range R>
requires std::ranges::viewable_range<R>
struct nested_loop_iterator {
public:
    using value_type = std::array<std::ranges::iterator_t<R>, N>;
    using difference_type = std::ptrdiff_t;

private:
    std::views::all_t<R> range_ {};
    value_type iters_ {};

public:
    explicit nested_loop_iterator(auto&& range)
            : range_(std::views::all(std::forward<decltype(range)>(range))),
              iters_() {
        std::ranges::fill(iters_, range_.begin());
    }

    value_type const& operator*() const { return iters_; }

    nested_loop_iterator& operator++() {
        for (auto& iter : iters_ | std::views::reverse) {
            if (++iter != range_.end()) {
                std::fill(&iter, iters_.end(), iter);
                return *this;
            }
        }
        return *this;
    }

    nested_loop_iterator operator++(int) & {
        auto temp = *this;
        ++*this;
        return temp;
    }
};

template <std::size_t N>
struct nested_loop_fn: public std::ranges::range_adaptor_closure<nested_loop_fn<N>> {
    template <std::ranges::forward_range R>
    static constexpr auto operator()(R&& range) {
        return std::ranges::subrange(
                nested_loop_iterator<N, R>(std::forward<R>(range)),
                std::unreachable_sentinel) |
               std::views::take_while(
                       [&range](
                               nested_loop_iterator<N, R>::value_type const& iter) {
                           return iter.front() != range.end();
                       });
    }
};

template <std::size_t N>
constexpr auto nested_loop = nested_loop_fn<N>{};

int main() {
    std::cout << is_prime(8147) << '\n';
    std::cout << is_prime(4817) << '\n';
    std::cout << is_prime(1487) << '\n';

    auto prime_groups =
            std::ranges::subrange(std::ranges::lower_bound(primes, 1000),
                                  std::ranges::upper_bound(primes, 9999)) |
            std::views::transform(
                    [](int prime) { return std::tuple{hash_prime(prime), prime}; }) |
            std::ranges::to<std::unordered_multimap>() |
            std::views::chunk_by([](auto&& lhs, auto&& rhs) {
                return std::get<0>(std::forward<decltype(lhs)>(lhs)) ==
                       std::get<0>(std::forward<decltype(rhs)>(rhs));
            }) |
            std::views::transform([](auto&& pack) {
                static_assert(
                        std::same_as<std::remove_cvref_t<
                                std::ranges::range_value_t<decltype(pack)>>,
                                std::pair<const int, int>>);
                return pack | std::views::transform([](auto&& ele) {
                    return std::get<1>(ele);
                }) |
                       std::ranges::to<std::unordered_set>();
            });

    for (auto const& group : prime_groups) {
        if (group.size() < 3) {
            continue;
        }
        for (auto [p1, p2] : group | nested_loop<2>) {
            int p3 = 2 * std::min(*p1, *p2) - std::max(*p1, *p2);
            if (group.count(p3) && p1 != p2) {
                std::cout << *p1 << ' ' << *p2 << ' ' << p3 << '\n';
            }
        }
    }
}