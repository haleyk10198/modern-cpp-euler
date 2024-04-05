#include <algorithm>
#include <any>
#include <boost/any.hpp>
#include <concepts>
#include <functional>
#include <generator>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>

template <typename T>
struct lattice_iterator {
public:
    using difference_type = std::ptrdiff_t;
    using value_type = std::vector<T>;
    using iterator_category = std::input_iterator_tag;

private:
    struct wrapped_iter {
        T value_;
        std::move_only_function<T(void)> adder_;

        wrapped_iter(const wrapped_iter&) = delete;
        wrapped_iter(wrapped_iter&&) = default;

        wrapped_iter(std::input_iterator auto&& iter) {
            value_ = *iter;
            adder_ = [iter_ = std::move(iter)]() mutable { return *++iter_; };
        }

        bool operator<(wrapped_iter const& rhs) const {
            return value_ < rhs.value_;
        }

        wrapped_iter& operator++() {
            value_ = adder_();
            return *this;
        }

        T& operator*() { return value_; }
    };

public:
    std::vector<wrapped_iter> iters_{};
    value_type values_{};

    template <std::input_iterator... I>
    requires(std::convertible_to<typename I::value_type, T> && ...)
    lattice_iterator(I&&... iters) {
        (iters_.emplace_back(std::forward<I>(iters)), ...);
        values_ =
                iters_ |
                std::views::transform([](auto&& iter) { return iter.value_; }) |
                std::ranges::to<std::vector>();
    }

    lattice_iterator(lattice_iterator const&) = delete;
    lattice_iterator(lattice_iterator&&) = default;
    lattice_iterator() = delete;

    lattice_iterator& operator=(lattice_iterator&& rhs) = default;

    value_type const& operator*() const { return values_; }

    lattice_iterator& operator++() {
        std::size_t min_idx =
                std::ranges::min_element(values_) - values_.begin();

        values_[min_idx] = *++iters_[min_idx];
        return *this;
    }

    void operator++(int) { ++*this; }
};

std::generator<int> get_triangles() {
    for (int n = 1;; ++n) {
        co_yield n*(n + 1) / 2;
    }
}

std::generator<long long> get_pentagonal() {
    for (long long n = 1;; ++n) {
        co_yield n * (3 * n - 1) / 2;
    }
}

std::generator<long long> get_hexagonal() {
    for (long long n = 1;; ++n) {
        co_yield n * (2 * n - 1);
    }
}

int main() {
    // auto tri = get_triangles();
    auto penta = get_pentagonal();
    auto hexa = get_hexagonal();
    auto lattice = std::ranges::subrange(
            lattice_iterator<long long>{penta.begin(), hexa.begin()},
            std::unreachable_sentinel);

    auto nums =
            std::move(lattice) | std::views::filter([](auto&& pack) -> bool {
                return pack.front() == pack.back();
            }) |
            std::views::transform([](auto&& pack) { return pack.front(); }) |
            std::views::take(3) | std::ranges::to<std::vector>();

    std::cout << nums.size() << '\n';

    std::cout << nums.back() << '\n';
}