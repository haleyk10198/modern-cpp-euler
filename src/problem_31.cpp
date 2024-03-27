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

long long int ways[300];

int main() {
    ways[0] = 1;

    for (auto penny : {1, 2, 5, 10, 20, 50, 100, 200}) {
        for (int sum = penny; sum <= 200; ++sum) {
            ways[sum] += ways[sum - penny];
        }
    }

    std::cout << ways[200] << '\n';

    return 0;
}
