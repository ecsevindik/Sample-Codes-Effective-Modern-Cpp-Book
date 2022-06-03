#include <memory>

class Widget {
public: 
    Widget();
    ~Widget(); // declare destructor here, default it in implementation file

    Widget(Widget&& rhs); // ditto
    Widget& operator=(Widget&& rhs); // ditto

    Widget(const Widget& rhs); // declare copy constructor here, implement it in the implementation file
    Widget& operator=(const Widget& rhs); //ditto

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};