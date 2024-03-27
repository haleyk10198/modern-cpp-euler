#include <algorithm>
#include <boost/functional/hash.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <charconv>
#include <chrono>
#include <generator>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <ranges>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <unordered_set>
#include <vector>

using std::literals::string_view_literals::operator""sv;
using boost::multiprecision::cpp_int;
using namespace std::chrono;

std::generator<int> to_prime_factors(int x) {
    for (int i = 2; i < x; ++i) {
        while (x % i == 0) {
            co_yield i;
            x /= i;
        }
    }

    if (x > 1) {
        co_yield x;
    }
}

inline struct {
    template <std::ranges::input_range R>
    std::map<std::ranges::range_value_t<R>, int> operator()(R&& range) {
        std::map<int, int> counter;
        for (auto&& ele : range) {
            counter[ele]++;
        }

        return counter;
    }
} to_counter;

template <typename K, std::integral V>
std::map<K, V>& operator*=(std::map<K, V>& subject, V scalar) {
    for (auto& [k, v] : subject) {
        v *= scalar;
    }

    return subject;
}

int main() {
    static_assert(
            std::same_as<int, std::ranges::range_value_t<std::generator<int>>>);

    auto as = std::views::iota(2, 101);
    auto bs = std::views::iota(2, 101);

    std::cout << std::ranges::size(
            std::views::cartesian_product(
                    as | std::views::transform(to_prime_factors) |
                    std::views::transform(to_counter),
                    bs) |
            std::views::transform([](auto&& pack) {
                auto& [factors, power] = pack;
                static boost::hash<decltype(factors)> hasher{};
                factors *= power;
                return hasher(factors);
            }) |
            std::ranges::to<std::unordered_set>())
              << '\n';

    return 0;
}
