#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>
#include <charconv>
#include <chrono>
#include <generator>
#include <iostream>
#include <map>
#include <mutex>
#include <ranges>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <vector>

using std::literals::string_view_literals::operator""sv;
using boost::multiprecision::cpp_int;
using namespace std::chrono;

constexpr auto PRIME_CHECK_BOUND = 2'000'010;
bool sieve[PRIME_CHECK_BOUND];
void sieving() {
    sieve[0] = sieve[1] = 1;
    for (int i = 2; i < PRIME_CHECK_BOUND; ++i) {
        if (!sieve[i]) {
            for (int j = i * 2; j < PRIME_CHECK_BOUND; j += i) {
                sieve[j] = 1;
            }
        }
    }
}

bool is_prime(int num) {
    static std::once_flag f1;
    std::call_once(f1, sieving);

    if(num < 0) {
        return false;
    }

    return !sieve[num];
}

template <typename R, typename Pred>
requires std::is_invocable_v<Pred, std::ranges::range_value_t<R>>
constexpr int count_while(R&& range, Pred pred) {
    return *(std::views::zip(std::views::iota(0), range) |
             std::views::drop_while([&pred](auto&& pack) {
                 auto& [_, ele] = pack;
                 return pred(ele);
             }) |
             std::views::elements<0>)
            .begin();
}

int prime_length(int a, int b) {
    return count_while(std::views::iota(0),
                       [a, b](int n) { return is_prime(n * n + a * n + b); });
}

int main() {
    auto as = std::views::iota(-999, 1000);
    auto bs = std::views::iota(-1000, 1001);
    auto abs = std::views::cartesian_product(as, bs);

    auto lengths = abs | std::views::transform([](auto&& pack) {
        auto [a, b] = pack;
        return std::tuple{prime_length(a, b), a * b};
    });

    std::cout << std::get<1>(*std::ranges::max_element(lengths)) << '\n';

    return 0;
}
