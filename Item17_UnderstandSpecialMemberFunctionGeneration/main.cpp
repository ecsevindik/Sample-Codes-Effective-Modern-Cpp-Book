
/*
• The special member functions are those compilers may generate on their own:
    default constructor, destructor, copy operations, and move operations.
• Move operations are generated only for classes lacking explicitly declared
    move operations, copy operations, and a destructor.
• The copy constructor is generated only for classes lacking an explicitly
    declared copy constructor, and it’s deleted if a move operation is declared.
    The copy assignment operator is generated only for classes lacking an explicitly
    declared copy assignment operator, and it’s deleted if a move operation is
    declared. Generation of the copy operations in classes with an explicitly
    declared destructor is deprecated.
• Member function templates never suppress generation of special member
    functions.
 */

/*
The C++11/14 rules governing the special member functions are thus:

• Default constructor: Same rules as C++98. Generated only if the class contains
no user-declared constructors.

• Destructor: Essentially same rules as C++98; sole difference is that destructors
are noexcept by default (see Item 14). As in C++98, virtual only if a base class
destructor is virtual.

• Copy constructor: Same runtime behavior as C++98: memberwise copy construction
of non-static data members. Generated only if the class lacks a userdeclared
copy constructor. Deleted if the class declares a move operation.
Generation of this function in a class with a user-declared copy assignment operator
or destructor is deprecated.

• Copy assignment operator: Same runtime behavior as C++98: memberwise
copy assignment of non-static data members. Generated only if the class lacks a
user-declared copy assignment operator. Deleted if the class declares a move
operation. Generation of this function in a class with a user-declared copy constructor
or destructor is deprecated.
• Move constructor and move assignment operator: Each performs memberwise
moving of non-static data members. Generated only if the class contains no userdeclared
copy operations, move operations, or destructor.
*/

#include <atomic>
#include <iostream>

class Widget {
public:
    Widget() {
        std::cout << "No arg constructor" << std::endl;
    }

    ~Widget() {
        std::cout << "Destructor" << std::endl;
    }

    Widget(const Widget& rhs) 
    : x(rhs.x), b{rhs.b.load()}{
        std::cout << "Copy constructor" << std::endl;
    }
    
    Widget& operator=(const Widget& rhs) {
        x = rhs.x; 
        b = rhs.b.load();
        std::cout << "Copy assignment" << std::endl;
        return *this;
    }

    Widget(Widget&& rhs)
    : x(std::move(rhs.x)), b{rhs.b.load()} {
        std::cout << "Move constructor" << std::endl;
    }

    Widget& operator=(Widget&& rhs) {
        x = std::move(rhs.x); 
        b = rhs.b.load();
        std::cout << "Move assignment" << std::endl;
        return *this;
    }

private:
    int x;
    std::atomic<bool> b; // std::atomic is neither copyable nor movable.
};

int main() {

    Widget w1, w2; // Default constructor
    Widget w3(w1); // Copy constructor
    w2 = w3; // Copy assignment 

    Widget w4(std::move(w2)); // move constructor. If the move operations are not defined while copy operations are, this line becomes copy constructor
    w4 = std::move(w1); // move assignment. If the move operations are not defined while copy operations are, this line becomes copy assignment

    return 0;
}