
/**
• decltype almost always yields the type of a variable or expression without
any modifications.
• For lvalue expressions of type T other than names, decltype always reports a
type of T&.
• C++14 supports decltype(auto), which, like auto, deduces a type from its
initializer, but it performs the type deduction using the decltype rules.
 */

/* Decltype exactly does what it meant to

    const int i = 0; // decltype(i) is const int

    bool f(const Widget& w); // decltype(w) is const Widget&; decltype(f) is bool(const Widget&)

    struct Point {
    int x, y;       // decltype(Point::x) is int
    };              // decltype(Point::y) is int

    Widget w;       // decltype(w) is Widget
    if (f(w)) …     // decltype(f(w)) is bool

    template<typename T> // simplified version of std::vector
    class vector {
    public:
    …
    T& operator[](std::size_t index);
    …
    };
    vector<int> v;  // decltype(v) is vector<int>
    …
    if (v[0] == 0) … // decltype(v[0]) is int&

*/

/*
template<typename Container, typename Index> // works, but requires refinement
auto authAndAccess(Container& c, Index i) -> decltype(c[i]) // Here, auto is not use for template type deduction, it is used with trailing return type syntax (->)
{
    authenticateUser();
    return c[i];
}
*/

#include <iostream>
#include <deque>

template<typename Container, typename Index> // C++14; not quite correct. Auto is used for template type deduction.
auto authAndAccessWrong(Container& c, Index i) // operator [] usually returns  T&. auto will deduce the return type as T since reference-ness is ignored during deduction
{ 
    // authenticateUser();
    return c[i]; // return type deduced from c[i]
}

template<typename Container, typename Index> // C++14; correct. Auto is used for template type deduction.
decltype(auto) authAndAccessCorrect(Container& c, Index i) // operator [] usually returns  T&. decltype(auto) will deduce the return type as T&
{ 
    // authenticateUser();
    return c[i]; // return type deduced from c[i]
}

int main() {

    std::deque<int> d;
    d.emplace_back(3);
    d.emplace_back(5);
    // authAndAccessWrong(d, 1) = 10; // Won't compile. Assignment to an rvalue is forbidden;
    authAndAccessCorrect(d, 1) = 10;

    if(authAndAccessCorrect(d,1) != 10) {
        std::cerr << "Wrong result on authAndAccessCorrect(d,1)" << std::endl;
    } else {
        std::cout << "Expected result = 10, actual result = " << authAndAccessCorrect(d,1) << std::endl;
    }


    auto x = authAndAccessCorrect(d,0); // type of x is int
    x = 123; // no effect on container
    if(authAndAccessCorrect(d,0) != 3) {
        std::cerr << "Wrong result on authAndAccessCorrect(d,0)" << std::endl;
    } else {
        std::cout << "Expected result = 3, actual result = " << authAndAccessCorrect(d,0) << std::endl;
    }


    decltype(auto) y = authAndAccessCorrect(d,0); // type of y is int&
    y = 124; // affects the container since it is a reference
    if(authAndAccessCorrect(d,0) != 124) {
        std::cerr << "Wrong result on authAndAccessCorrect(d,0)" << std::endl;
    } else {
        std::cout << "Expected result = 124, actual result = " << authAndAccessCorrect(d,0) << std::endl;
    }

    return 0;
}