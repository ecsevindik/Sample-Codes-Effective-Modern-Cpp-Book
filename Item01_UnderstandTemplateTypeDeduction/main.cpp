
/**
• During template type deduction, arguments that are references are treated as
non-references, i.e., their reference-ness is ignored.
• When deducing types for universal reference parameters, lvalue arguments get
special treatment.
• When deducing types for by-value parameters, const and/or volatile arguments
are treated as non-const and non-volatile.
• During template type deduction, arguments that are array or function names
decay to pointers, unless they’re used to initialize references.
 */

#include "TypeDeduction.h"

/*
template<typename T>
void f(ParamType param);

f(expr); // deduce T and ParamType from expr
*/

/* Case 1: ParamType is a Reference or Pointer, but not a Universal Reference
1. If expr’s type is a reference, ignore the reference part.
2. Then pattern-match expr’s type against ParamType to determine T.
*/
void case1() {
    int x = 27; // x is an int
    const int cx = x; // cx is a const int
    const int& rx = x; // rx is a reference to x as a const int

    getTypeRef(x); // T is int, param's type is int&
    getTypeRef(cx); // T is const int, param's type is const int&
    getTypeRef(rx); // T is const int, param's type is const int&

    getTypeConstRef(x); // T is int, param's type is const int&
    getTypeConstRef(cx); // T is int, param's type is const int&
    getTypeConstRef(rx); // T is int, param's type is const int&

    const int *px = &x; // px is a ptr to x as a const int
    getTypePointer(&x); // T is int, param's type is int*
    getTypePointer(px); // T is const int, param's type is const int*
    getTypeConstPointer(&x); // T is int, param's type is const int*
    getTypeConstPointer(px); // T is int, param's type is const int*
    
    getTypeRef(px); // T is const int *, param's type is const int* &
    getTypeConstRef(px); // T is const int *, param's type is const int * & const
    getTypeByValue(px); // T is const int *, param's type is const int *
}

/* Case 2: ParamType is a Universal Reference
• If expr is an lvalue, both T and ParamType are deduced to be lvalue references.
That’s doubly unusual. First, it’s the only situation in template type deduction
where T is deduced to be a reference. Second, although ParamType is declared
using the syntax for an rvalue reference, its deduced type is an lvalue reference.
• If expr is an rvalue, the “normal” (i.e., Case 1) rules apply.
*/
void case2() {
    int x = 27; // as before
    const int cx = x; // as before
    const int& rx = x; // as before
    getTypeUniversalRef(x); // x is lvalue, so T is int&, param's type is also int&
    getTypeUniversalRef(cx); // cx is lvalue, so T is const int&, param's type is also const int&
    getTypeUniversalRef(rx); // rx is lvalue, so T is const int&, param's type is also const int&
    getTypeUniversalRef(27); // 27 is rvalue, so T is int, param's type is therefore int&&
}

/* Case 3: ParamType is Neither a Pointer nor a Reference
1. As before, if expr’s type is a reference, ignore the reference part.
2. If, after ignoring expr’s reference-ness, expr is const, ignore that, too. If it’s
volatile, also ignore that. (volatile objects are uncommon. They’re generally
used only for implementing device drivers. For details, see Item 40.)
*/
void case3() {
    int x = 27; // as before
    const int cx = x; // as before
    const int& rx = x; // as before
    getTypeByValue(x); // T's and param's types are both int
    getTypeByValue(cx); // T's and param's types are again both int
    getTypeByValue(rx); // T's and param's types are still both int
}

// return size of an array as a compile-time constant. (The
// array parameter has no name, because we care only about
// the number of elements it contains.)
template<typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept
{
    return N;
}

// An array decays into a pointer to its first element.
void arrayArguments() {
    const char name[] = "J. P. Briggs"; // name's type is const char[13]
    const char * ptrToName = name; // array decays to pointer
    getTypeByValue(name); // name is array, but T deduced as const char*
    getTypeByValue(ptrToName); // ditto

    std::cout << "Array size = " << arraySize(name) << std::endl;
}

// Dummy function
void someFunc(int, double) {}

// Function types can decay into function pointers
void functionArguments() {
    getTypeByValue(someFunc); // param deduced as ptr-to-func; type is void (*)(int, double)
    getTypeRef(someFunc); // param deduced as ref-to-func; type is void (&)(int, double)
}

int main() {
    std::cout << "CASE 1:" << std::endl;
    case1();

    std::cout << "\nCASE 2:" << std::endl;
    case2();

    std::cout << "\nCASE 3:" << std::endl;
    case3();

    std::cout << "\nARRAYS:" << std::endl;
    arrayArguments();

    std::cout << "\nFUNCTIONS:" << std::endl;
    functionArguments();
    return 0;
}