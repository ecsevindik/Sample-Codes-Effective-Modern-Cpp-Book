
/**
• Default by-reference capture can lead to dangling references.
• Default by-value capture is susceptible to dangling pointers (especially this),
and it misleadingly suggests that lambdas are self-contained.
 */

#include <vector>
#include <functional>
#include <algorithm>

#include "Chronometer.h"

using FuncContainer = std::vector<std::function<void(int)>>;

void test1() {
    FuncContainer funcs;

    funcs.emplace_back([](int value) {std::cout << value << std::endl;});
    funcs.emplace_back([](int value) {std::cout << value*2 << std::endl;});

    int value = 5;
    for(auto& f : funcs) {
        f(value);
        value++;
    }
}

void addDivisor(FuncContainer& container) {
    int divisor = 5;
    container.emplace_back([&divisor](int value) {std::cout << value/divisor << std::endl;}); // Expected to be dangling reference, however, in release mode it works correctly
    container.emplace_back([=](int value) {std::cout << (value*2)/divisor << std::endl;});
}

void addDivisorStatic(FuncContainer& container) {
    static int divisor = 5;
    container.emplace_back([=](int value) {std::cout << value/divisor << std::endl;}); // captures nothing, refers to above static
    container.emplace_back([&](int value) {std::cout << (value*2)/divisor << std::endl;}); // captures nothing, refers to above static
    divisor *= 5;
}

void test2() {
    FuncContainer funcs;
    addDivisor(funcs);
    int value = 5;
    for(auto& f : funcs) {
        f(value);
        value*=2;
    }

    funcs.clear();
    
    addDivisorStatic(funcs);
    for(auto& f : funcs) {
        f(value);
        value*=2;
    }
}

template<typename C>
void workWithContainer(const C& container) {
    int divisor = 5;

    if( std::all_of(std::begin(container), std::end(container),
        [&](const auto& value){return value % divisor == 0;})) {
        std::cout << "All of the elements are multiple of " << divisor << std::endl;
    } else {
        std::cout << "At least one of the elements is not multiple of " << divisor << std::endl;
    }
}

void test3() {
    std::vector<int> nums(2,5);
    workWithContainer(nums);
    nums[1] = 3;
    workWithContainer(nums);
}

class Widget {
    public:
    Widget() = default;
    ~Widget() = default;

    void addFunc(FuncContainer& funcs) const {
        funcs.emplace_back([=] (int value) {std::cout << value/divisor << std::endl;}); // Captures this pointer. When Widget object destroyed, it becomes dangling pointer
    }

    // void addFunc(FuncContainer& funcs) const {
    //     funcs.emplace_back([] (int value) {std::cout << value/divisor << std::endl;}); // compile error since there is no local divisor
    // }

    // void addFunc(FuncContainer& funcs) const {
    //     funcs.emplace_back([divisor] (int value) {std::cout << value/divisor << std::endl;}); // compile error since there is no local divisor
    // }

    void addFunc2(FuncContainer& funcs) const {
        auto thisCopy = this;
        funcs.emplace_back([thisCopy] (int value) {std::cout << value/thisCopy->divisor << std::endl;}); // Captures this pointer. When Widget object destroyed, it becomes dangling pointer
    }

    void addFunc3(FuncContainer& funcs) const {
        auto divisorCopy = divisor;
        funcs.emplace_back([divisorCopy] (int value) {std::cout << value/divisorCopy << std::endl;}); 
    }

    void addFunc4(FuncContainer& funcs) const {
        funcs.emplace_back([divisor = divisor] (int value) {std::cout << value/divisor << std::endl;}); // Best way 
    }

private:
    int divisor = 5;
};

void test4() {
    FuncContainer funcs;
    Widget w;
    w.addFunc(funcs);
    w.addFunc2(funcs);
    w.addFunc3(funcs);
    w.addFunc4(funcs);
    int value = 5;
    for(auto& f : funcs) {
        f(value);
        value*=2;
    }
}

int main() {

    test1();
    test2();
    test3();
    test4();
    return 0;
}