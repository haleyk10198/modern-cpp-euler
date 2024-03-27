#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>
#include <charconv>
#include <chrono>
#include <generator>
#include <iostream>
#include <map>
#include <mutex>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <vector>

using std::literals::string_view_literals::operator""sv;
using boost::multiprecision::cpp_int;
using namespace std::chrono;

template <int N>
constexpr auto factorial_v = []() consteval {
    if constexpr (N <= 1)
        return 1;
    else
        return N * factorial_v<N - 1>;
}();

int digits[10];

template <int I>
void solve(int perm_idx) {
    if constexpr (I == 10) {
        return;
    } else {
        int ff = factorial_v<9 - I>;
        int dd = perm_idx / ff;

        auto* span_st = digits + I;
        std::nth_element(span_st, span_st + dd, digits + 10);

        std::swap(*span_st, span_st[dd]);
        solve<I + 1>(perm_idx - ff * dd);
    }
}

int main() {
    std::iota(digits, digits + 10, 0);
    solve<0>(1e6 - 1);

    std::for_each(digits, digits + 10, [](int x) { std::cout << x; });
    return 0;
}
