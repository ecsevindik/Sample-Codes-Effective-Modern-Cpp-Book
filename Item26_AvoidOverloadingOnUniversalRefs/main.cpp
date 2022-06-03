
/*
• Overloading on universal references almost always leads to the universal reference
overload being called more frequently than expected.
• Perfect-forwarding constructors are especially problematic, because they’re
typically better matches than copy constructors for non-const lvalues, and
they can hijack derived class calls to base class copy and move constructors.
 */

#include <string>
#include <iostream>
#include <chrono>
#include <set>

using namespace std;

std::multiset<std::string> names;

template<typename T>
void logAndAdd(T&& name) {
    auto now = std::chrono::steady_clock::now();
    std::cout << now.time_since_epoch().count() << " - " << name << std::endl;
    names.emplace(std::forward<T>(name));
}

void logAndAdd(int idx) {
    auto now = std::chrono::steady_clock::now();
    std::cout << now.time_since_epoch().count() << " - " << idx << std::endl;
    names.emplace(std::to_string(idx));
}

void overloadingFuncTest() {
    std::string petName("Darla");
    logAndAdd(petName); // calss template one
    logAndAdd(std::string("Persephone")); // calss template one
    logAndAdd("Patty Dog"); // calls template one
    logAndAdd(22); // calls int one

    // short x = 3;
    // logAndAdd(x); // This one calls the template one since it does not exactly match with the overload taking int. That's why it doesn't compile. Uncomment it to see the compilation error.
}

class Person {
public:
    template<typename T>
    explicit Person(T&& n) // perfect forwarding ctor;
    : name(std::forward<T>(n)) {} // initializes data member
    
    explicit Person(int idx) // int ctor
    : name(std::to_string(idx)) {}

private:
    std::string name;
};

void forwardingCtorTest() {
    Person p("Nancy");
    // auto cloneOfP(p); // Since p is not const, forwarding ctor is called which result in compilation error.

    const Person a("Amilton");
    auto cloneOfA(a); // Compiles since 'a' is const, which calls the compiler generated copy ctor
}

// class SpecialPerson : public Person {
// public:
//     SpecialPerson() = default;

//     SpecialPerson(const SpecialPerson& rhs) // copy ctor: calls base class forwarding ctor causing compilation error
//     : Person(rhs) {}

//     SpecialPerson(SpecialPerson&& rhs) // move ctor: calls base class forwarding ctor causing compilation error
//     : Person(std::move(rhs)) {}

// };

int main() {
    overloadingFuncTest();
    forwardingCtorTest();

    return 0;
}
