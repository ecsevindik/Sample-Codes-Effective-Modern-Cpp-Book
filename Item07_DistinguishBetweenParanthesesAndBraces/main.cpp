
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

************* if you try to call a Widget constructor with zero arguments using the analogous syntax, you declare a function instead of an object
Widget w1(10); // call Widget ctor with argument 10
Widget w2(); // most vexing parse! declares a function named w2 that returns a Widget!
Widget w3{}; // calls Widget ctor with no args

************* Problem with std::intializer_list
class Widget {
public:
    Widget(int i, bool b);
    Widget(int i, double d); 
    Widget(std::initializer_list<long double> il); // added

    operator float() const; // convert to float
    ...
};

Widget w1(10, true); // uses parens and, as before, calls first ctor
Widget w2{10, true}; // uses braces, but now calls std::initializer_list ctor (10 and true convert to long double)
Widget w3(10, 5.0); // uses parens and, as before, calls second ctor
Widget w4{10, 5.0}; // uses braces, but now calls std::initializer_list ctor (10 and 5.0 convert to long double)

Widget w5(w4); // uses parens, calls copy ctor
Widget w6{w4}; // uses braces, calls std::initializer_list ctor (w4 converts to float, and float converts to long double)
Widget w7(std::move(w4)); // uses parens, calls move ctor
Widget w8{std::move(w4)}; // uses braces, calls std::initializer_list ctor (for same reason as w6)

*** Only if there’s no way to convert the types of the arguments in a braced initializer to
the type in a std::initializer_list do compilers fall back on normal overload
resolution.

// std::initializer_list element type is now std::string
Widget(std::initializer_list<std::string> il);
… // no implicit

*** Empty braces mean no arguments, not
an empty std::initializer_list:

Widget w1; // calls default ctor
Widget w2{}; // also calls default ctor
Widget w3(); // most vexing parse! declares a function!
Widget w4({}); // calls std::initializer_list ctor with empty list
Widget w5{{}}; // ditto
*/


#include <iostream>

int main() {
    std::cerr << "This item has no sample code" << std::endl;
    return 0;
}