
/*
• Alternatives to the combination of universal references and overloading
include the use of distinct function names, passing parameters by lvaluereference-
to-const, passing parameters by value, and using tag dispatch.
• Constraining templates via std::enable_if permits the use of universal references
and overloading together, but it controls the conditions under which
compilers may use the universal reference overloads.
• Universal reference parameters often have efficiency advantages, but they typically
have usability disadvantages.
 */

#include <string>
#include <iostream>
#include <chrono>
#include <set>

using namespace std;

std::multiset<std::string> names;

void logAndAddImpl(int idx, std::true_type) {
    auto now = std::chrono::steady_clock::now();
    std::cout << now.time_since_epoch().count() << " - " << idx << std::endl;
    names.emplace(std::to_string(idx));
}

template<typename T>
void logAndAddImpl(T&& name, std::false_type) {
    auto now = std::chrono::steady_clock::now();
    std::cout << now.time_since_epoch().count() << " - " << name << std::endl;
    names.emplace(std::forward<T>(name));
}

// TAG DISPATCH
template<typename T>
void logAndAdd(T&& name) {
    logAndAddImpl( std::forward<T>(name), 
                   std::is_integral<std::remove_reference_t<T>>()
    );
}

void tagDispatchTest() {
    std::string petName("Darla"); 
    logAndAdd(petName); // calls false type
    logAndAdd(std::string("Persephone")); // calls false type
    logAndAdd("Patty Dog"); // calls false type
    logAndAdd(22); // calls true type

    short x = 3;
    logAndAdd(x); // calls true type
}

// TYPE TRAITS
class Person {
public:
    template<
        typename T, 
        typename =  std::enable_if_t<
                        !std::is_base_of<Person,std::decay_t<T>>::value // Prevents calling with Person or SpecialPerson
                        &&
                        !std::is_integral<std::remove_reference_t<T>>::value // Prevents calling with short, long, int types
                    >
        >
    explicit Person(T&& n) // perfect forwarding ctor;
    : name(std::forward<T>(n)) { // initializes data member
    
        // assert that a std::string can be created from a T object
        static_assert(
            std::is_constructible<std::string, T>::value,
            "Parameter n can't be used to construct a std::string"
        );

    } 
    
    explicit Person(int idx) // int ctor
    : name(std::to_string(idx)) {}

private:
    std::string name;
};

class Widget{
public:
    Widget() = default;
};

void ctorTest() {
    Person p1("Nancy"); // calls perfect forwarding ctor
    auto cloneOfP1(p1); // calls compiler generated copy constructor

    Person p2(5); // calls int ctor
    auto cloneOfP2(p2); // calls compiler generated copy constructor

    // Widget w;
    // Person p3(w); // static assertion fails here
}

class SpecialPerson : public Person {
public:

    SpecialPerson(const SpecialPerson& rhs) // copy ctor: calls base class compiler generated copy constructor
    : Person(rhs) {}

    SpecialPerson(SpecialPerson&& rhs) // move ctor: calls base class compiler generated move constructor
    : Person(std::move(rhs)) {}

};

int main() {
    tagDispatchTest();
    ctorTest();

    return 0;
}
