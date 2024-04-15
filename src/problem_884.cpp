#include <algorithm>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <cmath>
#include <functional>

std::size_t floor_cubic(std::size_t num) {
    auto root = static_cast<std::size_t>(std::cbrtl(num));
    if (root * root * root == num) --root;
    return root * root * root;
}

std::size_t sum(std::size_t tail, bool caches = false) {
    static std::unordered_map<std::size_t, std::size_t> cache_;
    if (cache_.count(tail) || tail <= 1) {
        return cache_[tail];
    }
    if (tail <= 1000) caches = true;

    auto sum_ = 0uz;

    auto floor = floor_cubic(tail);

    sum_ += sum(floor);
    sum_ += sum(tail - floor);

    sum_ += tail - floor;

    if (caches) { cache_[tail] = sum_; }

    return sum_;
}

constexpr std::size_t d(std::size_t num) {
    return num ? 1 + d(num - floor_cubic(num)) : 0;
}

constexpr std::size_t BOUND = 1e17;

int main() {
//    std::cout << std::ranges::fold_left(std::views::iota(1uz, BOUND) | std::views::transform(d), 0, std::plus<>())
//        << '\n';

    std::ranges::for_each(std::views::iota(1uz) |
                          std::views::transform([](auto &&num) { return static_cast<std::size_t>(std::pow(num, 3)); })
                          | std::views::take_while([](auto &&num) { return num < BOUND; }),
                          [](auto &&num) {
                              sum(num, true);
                          });

    std::cout << sum(BOUND) << '\n';
    return 0;
}