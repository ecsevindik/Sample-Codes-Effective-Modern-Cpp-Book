
/*
• If a function template parameter has type T&& for a deduced type T, or if an
object is declared using auto&&, the parameter or object is a universal reference.
• If the form of the type declaration isn’t precisely type&&, or if type deduction
does not occur, type&& denotes an rvalue reference.
• Universal references correspond to rvalue references if they’re initialized with
rvalues. They correspond to lvalue references if they’re initialized with lvalues.
 */

#include <string>
#include <iostream>
#include <vector>

using namespace std;

/* Samples

• void f(Widget&& param); // rvalue reference

• Widget&& var1 = Widget(); // rvalue reference

• auto&& var2 = var1; // universal reference

• template<typename T>
  void f(std::vector<T>&& param); // rvalue reference

• template<typename T>
  void f(T&& param); // universal reference

• template<typename T>
  void f(const T&& param); // param is an rvalue reference

• template<class T, class Allocator = allocator<T>> // from C++ Standards
  class vector { // Standards
  public:
    void push_back(T&& x); // rvalue reference since there is no type deduction. T is known when std::vector<T> is called.

• template<class T, class Allocator = allocator<T>> // from C++ Standards
  class vector {
  public:
    template <class... Args>
    void emplace_back(Args&&... args); // universal reference since Args are not known until emplace_back is called

• auto timeFuncInvocation =
    [](auto&& func, auto&&... params) // C++14, universal references
    {
        start timer;
        std::forward<decltype(func)>(func)( // invoke func
        std::forward<decltype(params)>(params)... // on params
        );
        stop timer and record elapsed time;
    };

 */

class Widget {
public:
    Widget(int x) : m_x(x) {};
    ~Widget() {std::cout << "Widget destructor is called " << m_x << std::endl;}

    Widget(Widget&& rhs)
    : m_x(std::move(rhs.m_x)){ m_x += 5;}

private:
    int m_x;
};

void WidgetConsumer(Widget&& w) {
    Widget wC(std::move(w));
    std::cout << "Widget consumed" << std::endl;
}

void test1(){
    Widget var1(1);
    Widget&& var2 = Widget(2);
    
    WidgetConsumer(std::move(var1));
    std::cout << "End of test1" << std::endl;
}

void randomFunc(int x, std::string y) {
    std::cout << "Random function called with x=" << x << " | y=" << y << std::endl;
}

void test2() {
    auto randomFuncCaller = [](auto&& func, auto&&... params) {
        std::forward<decltype(func)>(func)( // invoke func
        std::forward<decltype(params)>(params)... // on params
        );
    };

    randomFuncCaller(randomFunc, 3, "helloe");
}

int main() {

    test1();
    test2();

    return 0;
}