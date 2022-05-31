
/**
• std::move performs an unconditional cast to an rvalue. In and of itself, it
doesn’t move anything.
• std::forward casts its argument to an rvalue only if that argument is bound
to an rvalue.
• Neither std::move nor std::forward do anything at runtime.
 */

#include <string>
#include <iostream>

using namespace std;

// Rough move implementation
template<typename T>
decltype(auto) moveRough(T&& param)
{
    using ReturnType = remove_reference_t<T>&&;
    return static_cast<ReturnType>(param);
}

void process(const std::string& lvalArg) {
    std::cout << lvalArg << std::endl;
}

void process(std::string&& rvalArg) {
    std::cout << rvalArg << std::endl;
}

template < typename T>
void logAndProcess(T&& param) {
    std::cout << "Calling process " << std::endl;
    process(std::forward<T>(param));
}

// Same classes with different move operation
class Widget1 {
    public:
    Widget1() = default;
    ~Widget1() = default;

    Widget1(Widget1&& rhs) 
    : s(std::forward<std::string>(rhs.s)){
        ++moveCtorCalls;
    }

    void print(){ std::cout << moveCtorCalls << std::endl;}

private:
    static std::size_t moveCtorCalls;
    std::string s;
};

class Widget2 {
    public:
    Widget2() = default;
    ~Widget2() = default;
    
    Widget2(Widget2&& rhs) 
    : s(std::move(rhs.s)){
        moveCtorCalls = rhs.moveCtorCalls;
        ++moveCtorCalls;
    }

    void print(){ std::cout << moveCtorCalls << std::endl;}

private:
    static std::size_t moveCtorCalls;
    std::string s;
};

void test1() {
    std::string st1 = "Scott";
    std::string st2 = "Meyers";

    logAndProcess(st1);
    logAndProcess(std::move(st2));
}

std::size_t Widget1::moveCtorCalls = 0;
std::size_t Widget2::moveCtorCalls = 0;

void test2() {
    Widget1 w1_1;

    Widget1 w1_2(std::move(w1_1));
    w1_2.print();

    Widget2 w2_1;
    Widget2 w2_2(std::move(w2_1));
    Widget2 w2_3(std::move(w2_2));

    w2_3.print();
}

int main() {

    test1();
    test2();

    return 0;
}