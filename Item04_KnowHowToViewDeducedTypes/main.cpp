
/**
• Deduced types can often be seen using IDE editors, compiler error messages,
and the Boost TypeIndex library.
• The results of some tools may be neither helpful nor accurate, so an understanding
of C++’s type deduction rules remains essential.
 */

#include <vector>

#include "TypeDeduction.h"

template <typename T> // Declaration only for TD; TD == "Type Displayer"
class TD;

void testTypeId() {
    const int theAnswer = 42;

    auto x = theAnswer;
    auto y = &theAnswer;

    // TD<decltype(x)> xType; // Since there is no definition, compiler will give error while showing the type of x
    // TD<decltype(y)> yType; // Since there is no definition, compiler will give error while showing the type of y

    std::cout << typeid(x).name() << std::endl;
    std::cout << typeid(y).name() << std::endl;
}

template<typename T>
void typeInfo(const T& param) {
    std::cout << "T = " << typeid(T).name() << '\n'; // show T
    std::cout << "param = " << typeid(param).name() << '\n'; // show param's type
}

class Widget {
public:
    Widget() = default;
};

std::vector<Widget> createVec() {
    std::vector<Widget> v;
    v.resize(10);
    return v;
}

void testBoostTypeIndex() {
    const auto vw = createVec();
    if(!vw.empty()) {
        typeInfo(&vw[0]);

        /*
        Sadly, the results of std::type_info::name are not reliable. In this case, for example,
        the type that all three compilers report for param are incorrect. Furthermore,
        they’re essentially required to be incorrect, because the specification for std::
        type_info::name mandates that the type be treated as if it had been passed to a template
        function as a by-value parameter. As Item 1 explains, that means that if the type
        is a reference, its reference-ness is ignored, and if the type after reference removal is
        const (or volatile), its constness (or volatileness) is also ignored. That’s why
        param’s type—which is const Widget * const &—is reported as const Widget*.
        */

        // TD<decltype(&vw[0])> t; // This one is also wrong
        getTypeConstRef(&vw[0]); // Result of Boost.TypeIndex is accurate
    }
}

int main() {

    std::cout << "Test type id:" << std::endl;
    testTypeId();
    
    std::cout << std::endl << "Test boost typeindex:" << std::endl;
    testBoostTypeIndex();
    return 0;
}