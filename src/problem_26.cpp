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

std::generator<int> fraction_to_decimal(int denom) {
    for (int numer = 1, deci; numer; numer *= 10) {
        deci = numer / denom;
        numer -= deci * denom;
        co_yield numer;
    }
}

int to_cycle_length(int denom) {
    std::unordered_map<int, int> numers;
    for (auto&& [idx, numer] :
            std::views::zip(std::views::iota(0), fraction_to_decimal(denom))) {
        if (!numers.count(numer)) {
            numers[numer] = idx;
        } else {
            // std::cout << denom << ' ' << idx - numers[numer] << '\n';
            return idx - numers[numer];
        }
    }
    return 0;
}

int main() {
    auto ans = std::ranges::max_element(
            std::views::iota(1, 1000), {},
            [](auto&& num) { return to_cycle_length(num); });
    std::cout << *ans << '\n';

    // std::cout << to_cycle_length(11) << '\n';

    // std::ranges::for_each(fraction_to_decimal(11),
    //                       [](int num) { std::cout << num; });
    return 0;
}
