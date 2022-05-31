
/*
• Reference collapsing occurs in four contexts: template instantiation, auto type
generation, creation and use of typedefs and alias declarations, and
decltype.
• When compilers generate a reference to a reference in a reference collapsing
context, the result becomes a single reference. If either of the original references
is an lvalue reference, the result is an lvalue reference. Otherwise it’s an
rvalue reference.
• Universal references are rvalue references in contexts where type deduction
distinguishes lvalues from rvalues and where reference collapsing occurs.
 */


#include <iostream>

using namespace std;

/* Explanation

• template<typename T>
  void func(T&& param);

• Widget widgetFactory(); // function returning rvalue

• Widget w; // a variable (an lvalue)

• func(w); // call func with lvalue; T deduced to be Widget&
• func(Widget& && param) ---> reference collapsing ---> func(Widget& param)

• func(widgetFactory()); // call func with rvalue; T deduced to be Widget
• func(Widget&& param) ---> no reference collapsing ---> func(Widget&& param)

• auto&& w1 = w; // w1 initialized with lvalue, thus deducing they type Widget& for auto
• Widget& && w1 = w; ---> reference collapsing ---> Widget& w1 = w;

• auto&& w2 = widgetFactory(); // w2 initialized with rvalue, causing non-reference type Widget to be deduced for auto.
• Widget&& w2 = w; ---> no reference collapsing ---> w2 is a rvalue ref

• typedef T&& RefToT
• When T = int& ---> typedef int& && RefToT ---> reference collapsing ---> typedef int& RefToT
• Same thing applies to alias and decltype as well

*/

int main() {
    std::cerr << "No example code for this item. Check explanation part" << std::endl;
    return 0;
}