#include <algorithm>
#include <any>
#include <cmath>
#include <concepts>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <numeric>
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

template <std::size_t N, std::ranges::forward_range R>
    requires std::ranges::viewable_range<R>
struct nested_loop_iterator {
   public:
    using value_type = std::array<std::ranges::iterator_t<R>, N>;
    using difference_type = std::ptrdiff_t;

   private:
    std::views::all_t<R> range_{};
    value_type iters_{};

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
struct nested_loop_fn
    : public std::ranges::range_adaptor_closure<nested_loop_fn<N>> {
    template <std::ranges::forward_range R, typename P>
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
    is_prime(2);
    std::vector<long long int> prefix_sum;

    std::partial_sum(primes.begin(), primes.end(),
                     std::back_inserter(prefix_sum), std::plus());

    std::cout << prefix_sum.front() << '\n';

    prefix_sum | std::views::enumerate | std::views::transform([&prefix_sum](auto&& pack) {
        auto [idx, s1] = pack;
        return std::ranges::subrange(prefix_sum.begin()+1+idx, prefix_sum.end())
            | std::views::take_while([&s1](int s2) { return s2-s1 < 1'000'000; })
            | std::views::filter()
    })

    std::cout << std::ranges::max(prefix_sum | nested_loop<2> |
                                  std::views::filter([](auto&& pack) {
                                      auto [x, y] = pack;
                                      return is_prime(*y - *x);
                                  }) |
                                  std::views::transform([](auto&& pack) {
                                      auto [x, y] = pack;
                                      return std::distance(x, y);
                                  }))
              << '\n';

      return 0;
}