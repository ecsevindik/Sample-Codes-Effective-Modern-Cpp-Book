
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

// TAG DISPATCH
template<typename T>
void logAndAdd(T&& name) {
    logAndAddImpl( std::forward<T>(name), 
                   std::is_integral<std::remove_reference_t<T>>()
    );
}

template<typename T>
void logAndAddImpl(T&& name, std::false_type) {
    auto now = std::chrono::steady_clock::now();
    std::cout << now.time_since_epoch().count() << " - " << name << std::endl;
    names.emplace(std::forward<T>(name));
}


void logAndAddImpl(int idx, std::true_type) {
    auto now = std::chrono::steady_clock::now();
    std::cout << now.time_since_epoch().count() << " - " << idx << std::endl;
    names.emplace(std::to_string(idx));
}

void test1() {
    std::string petName("Darla");
    logAndAdd(petName);
    logAndAdd(std::string("Persephone"));
    logAndAdd("Patty Dog");
    logAndAdd(22);

    short x = 3;
    logAndAdd(x);
}

// TYPE TRAITS
class Person {
public:
    template<
        typename T, 
        typename =  std::enable_if_t<
                        !std::is_base_of<Person,std::decay_t<T>>::value // Prevents calling with Person or SpecialPerson
                        &&
                        !std::is_integral<std::remove_reference_t<T>>::value // Prevents calling with soth, long, int types
                    >
        >
    explicit Person(T&& n) // perfect forwarding ctor;
    : name(std::forward<T>(n)) {} // initializes data member
    
    explicit Person(int idx) // int ctor
    : name(std::to_string(idx)) {}

private:
    std::string name;
};

void test2() {
    Person p1("Nancy");
    auto cloneOfP1(p1);

    Person p2(5);
    auto cloneOfP2(p2);
}

class SpecialPerson : public Person {
public:

    SpecialPerson(const SpecialPerson& rhs) // copy ctor: calls base class compiler generated copy constructor
    : Person(rhs) {}

    SpecialPerson(SpecialPerson&& rhs) // move ctor: calls base class compiler generated move constructor
    : Person(std::move(rhs)) {}

};

int main() {
    test1();
    test2();

    return 0;
}