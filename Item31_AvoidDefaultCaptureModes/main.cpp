
/*
• Default by-reference capture can lead to dangling references.
• Default by-value capture is susceptible to dangling pointers (especially this),
and it misleadingly suggests that lambdas are self-contained.
 */

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

using FuncContainer = std::vector<std::function<void(int)>>;

void lambdaFuncTest() {
    std::cout << "lambdaFuncTest" << std::endl;

    FuncContainer funcs;

    funcs.emplace_back([](int value) {std::cout << value << std::endl;});
    funcs.emplace_back([](int value) {std::cout << value*2 << std::endl;});

    int value = 5;
    for(auto& f : funcs) {
        f(value);
        value++;
    }

    std::cout << std::endl;
}

void addDivisor(FuncContainer& container) {
    int divisor = 5;
    container.emplace_back([&divisor](int value) {std::cout << value/divisor << std::endl;}); // Expected to be dangling reference, however, in release mode it works correctly
    container.emplace_back([=](int value) {std::cout << (value*2)/divisor << std::endl;});
}

void addDivisorStatic(FuncContainer& container) {
    static int divisor_static = 5;
    container.emplace_back([=](int value) {std::cout << value/divisor_static << std::endl;}); // captures nothing, refers to above static
    container.emplace_back([&](int value) {std::cout << (value*2)/divisor_static << std::endl;}); // captures nothing, refers to above static
    divisor_static *= 5;
}

void captureModeTest() {

    std::cout << "captureModeTest" << std::endl;
    FuncContainer funcs;
    addDivisor(funcs);
    int value = 5;
    for(auto& f : funcs) {
        f(value);
        value*=2;
    }
    // value is 20 here
    
    funcs.clear();
    
    addDivisorStatic(funcs); // After this line divisor_static becomes 25, and lambdas use 25 as divisor
    for(auto& f : funcs) {
        f(value);
        value*=2;
    }

    std::cout << std::endl;
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

void lambdaFuncTest2() {

    std::cout << "lambdaFuncTest2" << std::endl;

    std::vector<int> nums(2,5);
    workWithContainer(nums);
    nums[1] = 3;
    workWithContainer(nums);

    std::cout << std::endl;
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
        funcs.emplace_back([divisorCopy] (int value) {std::cout << value/divisorCopy << std::endl;}); // solution
    }

    void addFunc4(FuncContainer& funcs) const {
        funcs.emplace_back([divisor = divisor] (int value) {std::cout << value/divisor << std::endl;}); // Best way 
    }

private:
    int divisor = 5;
};

void danglingRefTest() {
    std::cout << "danglingRefTest" << std::endl;
    FuncContainer funcs;
    {
        Widget w;
        w.addFunc(funcs); // This adds lambda with dangling divisor at the end of the scope. It outputs 0 in debug mode, works in release mode
        w.addFunc2(funcs); // This adds lambda with dangling divisor at the end of the scope. It outputs 0 in debug mode, 1 in release mode, which are wrong
        w.addFunc3(funcs); // fine
        w.addFunc4(funcs); // best way
    }
    int value = 5;
    for(auto& f : funcs) {
        f(value);
        value*=2;
    }

    std::cout << std::endl;
}

int main() {

    lambdaFuncTest();
    captureModeTest();
    lambdaFuncTest2();
    danglingRefTest();
    return 0;
}
