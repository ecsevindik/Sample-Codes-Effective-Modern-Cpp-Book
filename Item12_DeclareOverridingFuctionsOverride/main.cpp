
/*
• Declare overriding functions override.
• Member function reference qualifiers make it possible to treat lvalue and
rvalue objects (*this) differently.
 */

/*
When override keyword is not used, the signatures of both base and derived class functions must be exactly same
*/

#include <iostream>

class Base {
public:
    virtual void doWork(){
        std::cout << "Base class function called" << std::endl;
    }

    virtual void doSomething() & {
        std::cout << "Base class doSomething & function called" << std::endl;
    }

    virtual void func(int x) {
        std::cout << "Base class func is called with arg " << x << std::endl;
    }
};

class Widget : public Base {
public:
    void doWork() override {
        std::cout << "Derived class doWork function called" << std::endl;
    }

    void doSomething() & override {
        std::cout << "Derived class doSomething & function called" << std::endl;
    }

    void doSomething() && {
        std::cout << "Derived class doSomething && function called" << std::endl;
    }

    void func(unsigned int x) { // When this function declared with override, it gives compilation error since the signature is different with base class
        std::cout << "Derived class func is called with arg " << x << std::endl;
    }
};

Widget makeWidget() {
    return Widget();
}

void test1() {
    std::unique_ptr<Base> upb = std::make_unique<Widget>();
    upb->doWork(); // Calls derived class doSomething

    Widget w;
    w.doSomething(); // Calls doSomething &

    makeWidget().doSomething(); // Calls doSomething &&

    int x = 3;
    unsigned int y = 5;

    upb->func(x); // Calls base class func
    w.func(y); // Calss derived class func
}

int main() {
    test1();
    return 0;
}