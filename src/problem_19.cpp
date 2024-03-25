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
#include <vector>

using std::literals::string_view_literals::operator""sv;
using boost::multiprecision::cpp_int;
using namespace std::chrono;

std::generator<std::chrono::year_month_weekday> first_sundays() {
    for (auto mm = 1901y / January; mm < 2001y / January; mm += months(1)) {
        co_yield mm / Sunday[1];
    }
}

int main() {
    auto ans = first_sundays() | std::views::filter([](auto&& candidate) {
        year_month_day ymd{sys_days{candidate}};

        return ymd.day() == 1d;
    }) |;
               std::ranges::to<std::vector>();

    std::cout << ans.size() << '\n';
    return 0;
}
