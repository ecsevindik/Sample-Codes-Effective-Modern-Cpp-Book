
/**
• “Invisible” proxy types can cause auto to deduce the “wrong” type for an initializing
expression.
• The explicitly typed initializer idiom forces auto to deduce the type you want
it to have.
 */

/*
    Though std::vector<bool> conceptually holds
    bools, operator[] for std::vector<bool> doesn’t return a reference to an element
    of the container (which is what std::vector::operator[] returns for every type
    except bool). Instead, it returns an object of type std::vector<bool>::reference
    (a class nested inside std::vector<bool>)
*/

#include <iostream>
#include <vector>

#include "TypeDeduction.h"

std::vector<bool> feat() {
    std::vector<bool> b;
    b.emplace_back(true);
    b.emplace_back(false);
    return b;
}

int main() {
    
    auto h1 = feat()[1]; // h1 type is not bool, it is std::vector<bool>::reference which is a hidden proxy for packed bits
    std::cout << "h1's type = " << type_id_with_cvr<decltype(h1)>().pretty_name() << std::endl << std::endl;

    auto h2 = static_cast<bool>(feat()[0]); // Now, h2 type is bool
    std::cout << "h2's type = " << type_id_with_cvr<decltype(h2)>().pretty_name() << std::endl << std::endl;

    return 0;
}