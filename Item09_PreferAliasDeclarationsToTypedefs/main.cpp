
/**
• typedefs don’t support templatization, but alias declarations do.
• Alias templates avoid the “::type” suffix and, in templates, the “typename”
prefix often required to refer to typedefs.
• C++14 offers alias templates for all the C++11 type traits transformations.
 */


#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

/*

// With typedef

template<typename T> // MyAllocList<T>::type is synonym for std::list<T, MyAlloc<T>>
struct MyAllocList {
    typedef std::list<T, MyAlloc<T>> type;
};
MyAllocList<Widget>::type lw; // client code

template<typename T>
class Widget { // Widget<T> contains
private: // a MyAllocList<T>
    typename MyAllocList<T>::type list; // as a data member
    …
};

// with alias

template<typename T>
using MyAllocList = std::list<T, MyAlloc<T>>; // as before
template<typename T>
class Widget {
private:
MyAllocList<T> list; // no "typename", no "::type"
…
};

// Diff between C++11 and 14

std::remove_const<T>::type // C++11: const T → T
std::remove_const_t<T> // C++14 equivalent
std::remove_reference<T>::type // C++11: T&/T&& → T
std::remove_reference_t<T> // C++14 equivalent
std::add_lvalue_reference<T>::type // C++11: T → T&
std::add_lvalue_reference_t<T> // C++14 equivalent

*/

//Samples
typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UPtrMapSS1;
using UPtrMapSS2 = std::unique_ptr<std::unordered_map<std::string, std::string>>;

// FP is a synonym for a pointer to a function taking an int and
// a const std::string& and returning nothing
typedef void (*FP1)(int, const std::string&); // typedef same meaning as above
using FP2 = void (*)(int, const std::string&); // alias declaration

void func(FP2 f) {
    f(7, "Kein Delorean");
}

void f1(int x, const std::string& y) {
    std::cout << x << " - " << y << std::endl;
}

int main() {

    func(f1);
    
    return 0;
}