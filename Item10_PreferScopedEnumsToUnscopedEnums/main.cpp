
/*
• C++98-style enums are now known as unscoped enums.
• Enumerators of scoped enums are visible only within the enum. They convert
to other types only with a cast.
• Both scoped and unscoped enums support specification of the underlying type.
The default underlying type for scoped enums is int. Unscoped enums have no
default underlying type.
• Scoped enums may always be forward-declared. Unscoped enums may be
forward-declared only if their declaration specifies an underlying type.
 */

#include <iostream>
#include <vector>
#include <tuple>

#include "enumtest.h"
#include "status.h"

enum ColorUnscoped { black, white, red };
enum class ColorScoped{ green, blue, gray };

void basicTest() {
    
    ColorScoped c = ColorScoped::blue;
    // ColorScoped sc = gray; // error

    auto white = false;
    auto red = true; // This should have given error according to the book but works both for gcc and clang
    // ColorUnscoped uc = white; // error
    ColorUnscoped uc = black;
}

std::vector<std::size_t> primeFactors(std::size_t x) { // func. returning prime factors of x
    std::cout << "primeFactors function, x is " << x << std::endl;
    return std::vector<std::size_t>();
}

void castingTest() {
    ColorUnscoped uc = red;
    ColorScoped sc = ColorScoped::blue;

    if(uc < 14.5) {
        auto factors = primeFactors(uc);
    }

    // if( sc < 14.5) { // Gives error since scoped enums prevent implicit conversions
    //     auto factors = primeFactors(sc); // Gives error since scoped enums prevent implicit conversions
    // }

    if( static_cast<double>(sc) < 14.5) { // odd code, but it is valid
        auto factors = primeFactors(static_cast<size_t>(sc)); // suspect, but it compiles
    }
}

void forwardDeclarationTest() {
    Status s = Status::good;
    Color c = brown;
    continueProcessing(s, c);
}

using UserInfo = std::tuple<std::string, std::string, std::size_t>; // name, email, reputation

enum UserInfoFields {uiName, uiEmail, uiReputation};

enum class UserInfoFieldsScoped {uiName, uiEmail, uiReputation};

template<typename E> // C++14, underlying typecasting function
constexpr auto
toUType(E enumerator) noexcept
{
    return static_cast<std::underlying_type_t<E>>(enumerator);
}

void typecastingTest() {
    UserInfo uInfo;
    auto val1 =std::get<1>(uInfo);
    auto val2 = std::get<uiEmail>(uInfo);
    // auto val3 = std::get<UserInfoFieldsScoped::uiEmail>(uInfo); // Does not compile since std::get requires std::size_t while scoped enums type is int and prevents conversion
    auto val3 = std::get<static_cast<std::size_t>(UserInfoFieldsScoped::uiEmail)>(uInfo); // Work around for above problem
    auto val4 = std::get<toUType(UserInfoFieldsScoped::uiEmail)>(uInfo); // another solution to above problem
}

int main() {
    basicTest();
    castingTest();
    forwardDeclarationTest();
    typecastingTest();
    return 0;
}
