
/**
• Braced initialization is the most widely usable initialization syntax, it prevents
narrowing conversions, and it’s immune to C++’s most vexing parse.
• During constructor overload resolution, braced initializers are matched to
std::initializer_list parameters if at all possible, even if other constructors
offer seemingly better matches.
• An example of where the choice between parentheses and braces can make a
significant difference is creating a std::vector<numeric type> with two
arguments.
• Choosing between parentheses and braces for object creation inside templates
can be challenging.
 */

/* Samples
*************
int x(0); // initializer is in parentheses
int y = 0; // initializer follows "="
int z{ 0 }; // initializer is in braces
int z = { 0 }; // initializer uses "=" and braces

*************
Widget w1; // call default constructor
Widget w2 = w1; // not an assignment; calls copy ctor
w1 = w2; // an assignment; calls copy operator=

*************
class Widget {
…
private:
    int x{ 0 }; // fine, x's default value is 0
    int y = 0; // also fine
    int z(0); // error! function declaration
};

************* uncopyable objects may be initialized using braces or parentheses, but not using “=”:
std::atomic<int> ai1{ 0 }; // fine
std::atomic<int> ai2(0); // fine
std::atomic<int> ai3 = 0; // error!

************* A novel feature of braced initialization is that it prohibits implicit narrowing conversions among built-in types
double x, y, z;
…
int sum1{ x + y + z }; // error! sum of doubles may not be expressible as int
int sum2(x + y + z); // okay (value of expression truncated to an int)
int sum3 = x + y + z; // ditto



*** Only if there’s no way to convert the types of the arguments in a braced initializer to
the type in a std::initializer_list do compilers fall back on normal overload
resolution.

*** Empty braces mean no arguments, not
an empty std::initializer_list:

*/


#include <iostream>

class Widget {
public:
    Widget() {
        std::cout << "Ctor with no args is called" << std::endl;
    }
    Widget(int i, bool b) {
        std::cout << "Ctor with (int,bool) is called" << std::endl;
    }
    Widget(int i, double d) {
        std::cout << "Ctor with (int,double) is called" << std::endl;
    }
    Widget(std::initializer_list<long double> il) {
        std::cout << "Ctor with (std::initializer_list<long double>) is called" << std::endl;
    }

    Widget(const Widget& rhs) {
        std::cout << "Copy constructor is called" << std::endl;
    }

    Widget(Widget&& rhs) noexcept {
        std::cout << "Move constructor is called" << std::endl;
    }

    operator float() const { // convert to float
        return 3.f;
    }
    
};

int main() {
    Widget w1; // calls default ctor
    Widget w2{}; // also calls default ctor
    // Widget w3(); // most vexing parse! declares a function!
    Widget w4({}); // calls std::initializer_list ctor with empty list
    Widget w5{{}}; // ditto
    
    std::cout << std::endl;

    Widget w6(10, true); // uses parens and, calls first ctor
    Widget w7{10, true}; // uses braces, but now calls std::initializer_list ctor (10 and true convert to long double)
    Widget w8(10, 5.0); // uses parens and, as before, calls second ctor
    Widget w9{10, 5.0}; // uses braces, but now calls std::initializer_list ctor (10 and 5.0 convert to long double)

    std::cout << std::endl;

    Widget w10(w4); // uses parens, calls copy ctor
    Widget w11{w4}; // uses braces, calls std::initializer_list ctor (w4 converts to float, and float converts to long double)
    Widget w12(std::move(w4)); // uses parens, calls move ctor
    Widget w13{std::move(w5)}; // uses braces, calls std::initializer_list ctor (for same reason as w6)
    return 0;
}