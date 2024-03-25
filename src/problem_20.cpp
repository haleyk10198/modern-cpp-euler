#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>
#include <charconv>
#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <ranges>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <vector>

using std::literals::string_view_literals::operator""sv;
using boost::multiprecision::cpp_int;
using namespace std::chrono;

int main() {
    auto ints_100 = std::views::iota(1) | std::views::take(100) |
                    std::views::transform([](auto&& x) { return cpp_int{x}; });
    cpp_int factor_100 =
            std::ranges::fold_left_first(
                    ints_100, [](auto lhs, auto rhs) -> cpp_int { return lhs * rhs; })
                    .value();
    int ans = std::ranges::fold_left(
            factor_100.str(), 0, [](int acc, char ch) { return acc + ch - '0'; });

    std::cout << ans << '\n';
    return 0;
}
