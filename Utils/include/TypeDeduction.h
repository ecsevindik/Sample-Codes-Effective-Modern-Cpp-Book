#include <iostream>
#include <boost/type_index.hpp>

using boost::typeindex::type_id_with_cvr;

/*
The following functions does not actually print the type of input. They print the deduced type of the input for that particular instantiation.
*/

template<typename T>
void getTypeConstRef(const T& param) {
    std::cout << "Type deduction by const T&" << std::endl;
    std::cout << "  T = " << type_id_with_cvr<T>().pretty_name() << std::endl;
    std::cout << "  ParamType = " << type_id_with_cvr<decltype(param)>().pretty_name() << std::endl << std::endl;
}

template<typename T>
void getTypeRef(T& param) {
    std::cout << "Type deduction by T&" << std::endl;
    std::cout << "  T = " << type_id_with_cvr<T>().pretty_name() << std::endl;
    std::cout << "  ParamType = " << type_id_with_cvr<decltype(param)>().pretty_name() << std::endl << std::endl;
}

template<typename T>
void getTypePointer(T* param) {
    std::cout << "Type deduction by T*" << std::endl;
    std::cout << "  T = " << type_id_with_cvr<T>().pretty_name() << std::endl;
    std::cout << "  ParamType = " << type_id_with_cvr<decltype(param)>().pretty_name() << std::endl << std::endl;
}

template<typename T>
void getTypeConstPointer(const T* param) {
    std::cout << "Type deduction by const T*" << std::endl;
    std::cout << "  T = " << type_id_with_cvr<T>().pretty_name() << std::endl;
    std::cout << "  ParamType = " << type_id_with_cvr<decltype(param)>().pretty_name() << std::endl << std::endl;
}

template<typename T>
void getTypeUniversalRef(T&& param) {
    std::cout << "Type deduction by T&&" << std::endl;
    std::cout << "  T = " << type_id_with_cvr<T>().pretty_name() << std::endl;
    std::cout << "  ParamType = " << type_id_with_cvr<decltype(param)>().pretty_name() << std::endl << std::endl;
}

template<typename T>
void getTypeByValue(T param) {
    std::cout << "Type deduction by T" << std::endl;
    std::cout << "  T = " << type_id_with_cvr<T>().pretty_name() << std::endl;
    std::cout << "  ParamType = " << type_id_with_cvr<decltype(param)>().pretty_name() << std::endl << std::endl;
}