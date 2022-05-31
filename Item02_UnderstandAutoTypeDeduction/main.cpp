
/**
• auto type deduction is usually the same as template type deduction, but auto
type deduction assumes that a braced initializer represents a std::initial
izer_list, and template type deduction doesn’t.
• auto in a function return type or a lambda parameter implies template type
deduction, not auto type deduction.
 */

#include "TypeDeduction.h"

// auto createInitList()
// {
//     return { 1, 2, 3 }; // error: can't deduce type for { 1, 2, 3 }
// } 


int main() {

    auto x = 27;  //int
    const auto cx = x; // const int
    const auto& rx = x; // const int&
    auto&& dx = x; // int&
    auto&& sx = cx; // const int&
    auto&& zx = rx; // const int&
    auto&& xx = 3; // int&&
    dx = xx; // implicitly x, rx and zx is changed here
    std::cout << "x = " << x << std::endl;
    getTypeRef(x);
    std::cout << "cx = " << cx << std::endl;
    getTypeRef(cx);
    std::cout << "rx = " << rx << std::endl;
    getTypeRef(rx);
    std::cout << "sx = " << sx << std::endl;
    getTypeRef(sx);
    std::cout << "zx = " << zx << std::endl;
    getTypeRef(zx);
    std::cout << "xx = " << xx << std::endl; // Although paramtype of xx is an rvalue, xx is lvalue by itself
    getTypeRef(xx);

    auto y = {27};
    std::cout << "y.begin() = " << *y.begin() << std::endl;
    getTypeRef(y);

    // auto x5 = { 1, 2, 3.0 }; // error! can't deduce T for std::initializer_list<T>
    // getTypeByValue({ 11, 23, 9 }); // error! can't deduce type for T

/*
template<typename T>
void f(std::initializer_list<T> initList);
f({ 11, 23, 9 }); // T deduced as int, and initList's type is std::initializer_list<int>
*/

    return 0;
}