
/*
• Prefer deleted functions to private undefined ones.
• Any function may be deleted, including non-member functions and template
instantiations.
 */

/*
By convention, deleted functions are declared public, not private. There’s a reason
for that. When client code tries to use a member function, C++ checks accessibility
before deleted status. When client code tries to use a deleted private function, some
compilers complain only about the function being private, even though the function’s
accessibility doesn’t really affect whether it can be used. It’s worth bearing this
in mind when revising legacy code to replace private-and-not-defined member
functions with deleted ones, because making the new functions public will generally
result in better error messages.
*/

#include <iostream>

bool isLucky(int num) {
    std::cout << "Lucky number is " << num << std::endl;
    return true;
}

bool isLucky(char) = delete;
bool isLucky(bool) = delete;
bool isLucky(double) = delete;

void overloadDeleteTest() {
    isLucky(3);
    // isLucky(true); // Does not compile since its overload is deleted, uncomment to see the error
    // isLucky('a'); // ditto
    // isLucky(3.5f); // ditto
}

template<typename T>
void processPointer(T* ptr) {}

// Way to prevent user from using certain types
template<>
void processPointer<void>(void*) = delete;
template<>
void processPointer<char>(char*) = delete;
template<>
void processPointer<const void>(const void*) = delete;
template<>
void processPointer<const char>(const char*) = delete;

class Widget {
public:

    Widget() = default;

    template<typename T>
    void processPointer(T* ptr)
    {
        std::cout << "processPointer input value is " << *ptr << std::endl;
    }
// private:
//     template<> // Book says declaring template function in private does not compile. It does not compile with Gcc, but it compiles with clang
//     void processPointer<void>(void*);
};


// another way to delete certain types for a template member function
template<>
void Widget::processPointer<char>(char*) = delete;

void templateOverloadTest() {
    Widget w;

    int x = 3;
    w.processPointer(&x);

    // char a = 'a';
    // w.processPointer(&a); // Does not compile since it is deleted. Uncomment it to see the error.
}

int main() {
    overloadDeleteTest();
    templateOverloadTest();
    return 0;
}