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
    auto res = std::ranges::fold_left(
            std::views::iota(1, 1000) | std::views::transform([](int num) {
                cpp_int pow = boost::multiprecision::pow(cpp_int(num), num);
                return pow;
            }),
            cpp_int{}, [](cpp_int const& lhs, cpp_int const& rhs) -> cpp_int {
                return lhs + rhs;
            });

    std::cout << res.str().substr(res.str().length() - 10, 10) << std::endl;
}