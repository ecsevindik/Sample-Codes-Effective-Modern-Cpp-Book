#include <vector>
#include <string>
#include <iostream>

#include "widget.h"

struct Widget::Impl {
    std::string name;
    std::vector<double> data;
};

Widget::Widget()
: pImpl(std::make_unique<Impl>()) {
    pImpl->name = "Hi ne dedin!!";
}

Widget::~Widget() = default; // This has to be defined here because compiler does not know the type of Impl to destroy unique_ptr in header file. This situation is not valid for shared_ptr
Widget::Widget(Widget&& rhs) = default; // ditto
Widget& Widget::operator=(Widget&& rhs) = default; //ditto
Widget::Widget(const Widget& rhs) 
: pImpl(std::make_unique<Impl>(*rhs.pImpl)) { // Cannot be defaulted due to unique_ptr being move-only type
    std::cout << "Dans et benle kanka" << std::endl;
}
Widget& Widget::operator=(const Widget& rhs) { // Cannot be defaulted due to unique_ptr being move-only type
    *pImpl = *rhs.pImpl;

    std::cout << pImpl->name << std::endl;
    return *this;
}