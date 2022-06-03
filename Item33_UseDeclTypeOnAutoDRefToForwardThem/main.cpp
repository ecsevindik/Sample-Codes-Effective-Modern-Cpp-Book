
/*
• Use decltype on auto&& parameters to std::forward them.
 */

#include <iostream>
#include <vector>

template<typename T>
T normalize1(T x) {
    return x/2.0;
}

template<typename T>
T normalize2(T x, T y) {
    return x/y;
}

template<typename T>
T func(T value) {
    std::cout << value << std::endl;
    return value * 2;
}

int main() {
    
    auto f1 = [] (auto&& param) {
        return func(normalize1(std::forward<decltype(param)> (param)));
    };

    auto f2 = [] (auto&&... params) {
        return func(normalize2(std::forward<decltype(params)> (params)...));
    };

    std::cout << f1(3.0) << std::endl;
    std::cout << f2(6.0, 3.0) << std::endl;

    return 0;
}
