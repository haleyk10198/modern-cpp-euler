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

std::generator<cpp_int> fibonacci() {
    for (cpp_int f1{1}, f2{1};; f2 += f1, f1 = f2 - f1) {
        co_yield f1;
    }
}

int main() {
    auto ans = std::views::zip(fibonacci(), std::views::iota(1)) |
               std::views::drop_while([](auto&& pack) {
                   auto& [num, _] = pack;
                   return num.str().length() < 1000;
               }) |
               std::views::take(1) | std::views::elements<1>;

    std::ranges::for_each(ans, [](auto&& idx) { std::cout << idx << '\n'; });
    return 0;
}
