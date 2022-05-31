
/*
• constexpr objects are const and are initialized with values known during
compilation.
• constexpr functions can produce compile-time results when called with
arguments whose values are known during compilation.
• constexpr objects and functions may be used in a wider range of contexts
than non-constexpr objects and functions.
• constexpr is part of an object’s or function’s interface.
 */

/*
int sz; // non-constexpr variable
…
constexpr auto arraySize1 = sz; // error! sz's value not known at compilation
std::array<int, sz> data1; // error! same problem
constexpr auto arraySize2 = 10; // fine, 10 is a compile-time constant
std::array<int, arraySize2> data2; // fine, arraySize2 is constexpr

• constexpr functions can be used in contexts that demand compile-time constants.
If the values of the arguments you pass to a constexpr function in such a
context are known during compilation, the result will be computed during
compilation. If any of the arguments’ values is not known during compilation,
your code will be rejected.
• When a constexpr function is called with one or more values that are not
known during compilation, it acts like a normal function, computing its result at
runtime. This means you don’t need two functions to perform the same operation,
one for compile-time constants and one for all other values. The constexpr
function does it all.

*/

#include <array>

#include "Chronometer.h"

namespace ecs {

constexpr int pow(int base, int exp) noexcept // C++14
{
    auto result = 1;
    for (int i = 0; i < exp; ++i) result *= base;

    return result;
}

}

void test1() {
    constexpr auto numConds = 5;
    std::array<int, ecs::pow(3, numConds)> results; // pow provides compile-time constant since inputs are also ctc.

    std::cout << results.size() << std::endl;

    int x = 3;
    int y = 4;
    // std::array<int, ecs::pow(x, y)> results2; // Does not compile since std::array needs compile-time constant while pow provides runtime value
    std::cout << ecs::pow(x, y) << std::endl;
}

class Point {
public:
    constexpr Point(double xVal = 0, double yVal = 0) noexcept
    : x(xVal), y(yVal)
    {}
    constexpr double xValue() const noexcept { return x; }
    constexpr double yValue() const noexcept { return y; }
    constexpr void setX(double newX) noexcept { x = newX; }
    constexpr void setY(double newY) noexcept { y = newY; }

private:
    double x, y;
};

constexpr
Point midpoint(const Point& p1, const Point& p2) noexcept
{
    return { (p1.xValue() + p2.xValue()) / 2, // call constexpr
                (p1.yValue() + p2.yValue()) / 2 }; // member funcs
}

// return reflection of p with respect to the origin (C++14)
constexpr Point reflection(const Point& p) noexcept
{
    Point result; // create non-const Point
    result.setX(-p.xValue()); // set its x and y values
    result.setY(-p.yValue());
    return result; // return copy of it
}

void test2() {
    constexpr Point p1(9.4, 27.7); // fine, "runs" constexpr ctor during compilation
    constexpr Point p2(28.8, 5.3); // also fine
    constexpr auto mid = midpoint(p1, p2); // init constexpr object w/result of constexpr function
    constexpr auto reflectedMid = reflection(mid); // reflectedMid's value is (-19.1 -16.5) and known during compilation

    std::array<int, static_cast<int>(mid.xValue())> results;
    std::cout << results.size() << std::endl;
}

int main() {
    test1();
    test2();
    return 0;
}