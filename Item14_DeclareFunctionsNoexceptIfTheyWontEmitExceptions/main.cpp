
/*
• noexcept is part of a function’s interface, and that means that callers may
depend on it.
• noexcept functions are more optimizable than non-noexcept functions.
• noexcept is particularly valuable for the move operations, swap, memory
deallocation functions, and destructors.
• Most functions are exception-neutral rather than noexcept.
 */

/*
int f(int x) throw(); // no exceptions from f: C++98 style
int f(int x) noexcept; // no exceptions from f: C++11 style

RetType function(params) noexcept; // most optimizable
RetType function(params) throw(); // less optimizable
RetType function(params); // less optimizable

template <class T, size_t N>
void swap(T (&a)[N],T (&b)[N]) noexcept(noexcept(swap(*a, *b)));

template <class T1, class T2>
struct pair {
    …
    void swap(pair& p) noexcept(noexcept(swap(first, p.first)) &&
    noexcept(swap(second, p.second)));
    …
};
These functions are conditionally noexcept: whether they are noexcept depends on
whether the expressions inside the noexcept clauses are noexcept. Given two arrays
of Widget, for example, swapping them is noexcept only if swapping individual elements
in the arrays is noexcept, i.e., if swap for Widget is noexcept. The author of
Widget’s swap thus determines whether swapping arrays of Widget is noexcept.
That, in turn, determines whether other swaps, such as the one for arrays of arrays of
Widget, are noexcept. Similarly, whether swapping two std::pair objects containing
Widgets is noexcept depends on whether swap for Widgets is noexcept. The
fact that swapping higher-level data structures can generally be noexcept only if
swapping their lower-level constituents is noexcept should motivate you to offer
noexcept swap functions whenever you can.

*/

#include <vector>

#include "Chronometer.h"

class Widget {
public:
    Widget() = default;
    ~Widget() = default;

    Widget(Widget&& rhs) noexcept(false) // Move constructor
    : a(std::move(rhs.a)) {}

    Widget& operator=(Widget&& rhs) noexcept(false) { // Move assignment
        a = std::move(rhs.a);
        return *this;
    }

    Widget(const Widget& rhs) : a(rhs.a) {} // Copy constructor
    Widget& operator=(const Widget& rhs) { // Copy assignment
        a = rhs.a;
        return *this;
    }

private:
    std::string a = "ASDKASKDKASKDAKSDK";
};

class WidgetNoExcept {
public:
    WidgetNoExcept() = default;
    ~WidgetNoExcept() = default;

    WidgetNoExcept(WidgetNoExcept&& rhs) noexcept // Move constructor
    : a(std::move(rhs.a)) {}
    WidgetNoExcept& operator=(WidgetNoExcept&& rhs) noexcept { // Move assignment
        a = std::move(rhs.a);
        return *this;
    }

    WidgetNoExcept(const WidgetNoExcept& rhs) : a(rhs.a) {} // Copy constructor
    WidgetNoExcept& operator=(const WidgetNoExcept& rhs) { // Copy assignment
        a = rhs.a;
        return *this;
    }
private:
    std::string a = "ASDKASKDKASKDAKSDK";
};

int main() {

    std::vector<Widget> vec1;
    vec1.reserve(100); // Vector reserve size is intentionally left small in order to make vector increase its size and copy/move its alements to new memory

    std::vector<WidgetNoExcept> vec2;
    vec2.reserve(100);

    utils::Chronometer ch;
    ch.Start();
    for(int i = 0; i < 10000; ++i) {
        WidgetNoExcept w;
        vec2.push_back(w);
    }
    ch.Stop();
    ch.Report("Noexcept true  ");

    ch.Start();
    for(int i = 0; i < 10000; ++i) {
        Widget w;
        vec1.push_back(w);
    }
    ch.Stop();
    ch.Report("Noexcept false ");

    return 0;
}
