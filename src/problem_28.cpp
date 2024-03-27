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

static constexpr auto SQUARE_SIZE = 1001;

std::generator<int> ring_sum() {
    co_yield 1;
    for (int x = 1, length = 2;; length += 2) {
        x += length * 5 / 2;
        co_yield x * 4;
        x += length * 3 / 2;
    }
}

int main() {
    std::cout << std::ranges::fold_left(
            ring_sum() | std::views::take((SQUARE_SIZE + 1) / 2), 0,
            std::plus<>())
              << std::endl;
    return 0;
}
