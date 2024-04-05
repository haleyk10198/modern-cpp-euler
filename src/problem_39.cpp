#include <ranges>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <complex>

inline struct {
    template<typename R>
    auto operator()(R &&range) {
        std::unordered_map<std::ranges::range_value_t<R>, int> counter;
        for (auto &&ele: range) {
            counter[ele]++;
        }

        return counter;
    }
} to_counter;

int main() {
    auto ps = std::views::iota(1, 500)
              | std::views::transform([](int a) {
        return std::views::iota(a, 1000-a)
               | std::views::transform([a](int b) {
            return std::tuple{b, static_cast<int> (std::sqrt(a * a + b * b))};
        }) | std::views::filter([a](auto &&pack) {
            auto [b, c] = pack;
            std::cout << a << ' ' << b << ' ' << c << std::endl;
            return a * a + b * b == c * c;
        }) | std::views::transform([a](auto &&pack) {
            auto [b, c] = pack;
            return a + b + c;
        }) | std::views::take_while([](int p) {
            return p <= 1000;
        });
    })
              | std::views::join;

    static_assert(std::same_as<int, std::ranges::range_value_t<decltype(ps)>>);

    auto counter = to_counter(ps);

    std::cout << std::ranges::max_element(counter, {}, [](auto &&pack) {
        return std::get<1>(std::forward<decltype(pack)>(pack));
    })->first
              << '\n';
}