
/**
• auto type deduction is usually the same as template type deduction, but auto
type deduction assumes that a braced initializer represents a std::initial
izer_list, and template type deduction doesn’t.
• auto in a function return type or a lambda parameter implies template type
deduction, not auto type deduction.
 */

#include "TypeDeduction.h"

/*  NOTES
auto createInitList()
{
     return { 1, 2, 3 }; // error: can't deduce type for { 1, 2, 3 }
}

The reason for compile error is that auto in function return implies template type deduction (TTD), and TTD cannot deduce {1,2,3} type.

*/

/*
    auto x5 = { 1, 2, 3.0 }; // error! can't deduce T for std::initializer_list<T>
    getTypeByValue({ 11, 23, 9 }); // error! can't deduce type for T
*/

/*
    template<typename T>
    void f(std::initializer_list<T> initList);
    f({ 11, 23, 9 }); // T deduced as int, and initList's type is std::initializer_list<int>
*/


// Dummy function
void someFunc(int, double) {} // someFunc is a function; type is void(int, double)


int main() {

    auto x = 27;  //int
    const auto cx = x; // const int
    const auto& rx = x; // const int&
    auto&& dx = x; // int&
    auto&& sx = cx; // const int&
    auto&& zx = rx; // const int&
    auto&& xx = 3; // int&&
    dx = xx; // implicitly x, rx and zx is changed here

    const char name[] = "R. N. Briggs";

    auto arr1 = name; // arr1's type is const char*
    auto& arr2 = name; // arr2's type is const char (&)[13]    

    auto func1 = someFunc; // func1's type is void (*)(int, double)
    auto& func2 = someFunc; // func2's type is void (&)(int, double)

    auto y = {27};

    std::cout << "x = " << x << " and its type = " << type_id_with_cvr<decltype(x)>().pretty_name() << std::endl << std::endl;
    std::cout << "cx = " << cx << " and its type = " << type_id_with_cvr<decltype(cx)>().pretty_name() << std::endl << std::endl;
    std::cout << "rx = " << rx << " and its type = " << type_id_with_cvr<decltype(rx)>().pretty_name() << std::endl << std::endl;
    std::cout << "sx = " << sx << " and its type = " << type_id_with_cvr<decltype(sx)>().pretty_name() << std::endl << std::endl;
    std::cout << "zx = " << zx << " and its type = " << type_id_with_cvr<decltype(zx)>().pretty_name() << std::endl << std::endl;
    std::cout << "xx = " << xx << " and its type = " << type_id_with_cvr<decltype(xx)>().pretty_name() << std::endl << std::endl;
    std::cout << "name = " << name << " and its type = " << type_id_with_cvr<decltype(name)>().pretty_name() << std::endl << std::endl;
    std::cout << "arr1 = " << arr1 << " and its type = " << type_id_with_cvr<decltype(arr1)>().pretty_name() << std::endl << std::endl;
    std::cout << "arr2 = " << arr2 << " and its type = " << type_id_with_cvr<decltype(arr2)>().pretty_name() << std::endl << std::endl;
    std::cout << "func1's type = " << type_id_with_cvr<decltype(func1)>().pretty_name() << std::endl << std::endl;
    std::cout << "func2's type = " << type_id_with_cvr<decltype(func2)>().pretty_name() << std::endl << std::endl;
    std::cout << "y's type = " << type_id_with_cvr<decltype(y)>().pretty_name() << std::endl << std::endl;

    return 0;
}