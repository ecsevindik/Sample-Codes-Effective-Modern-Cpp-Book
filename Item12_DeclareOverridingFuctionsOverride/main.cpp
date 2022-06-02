
/*
• Declare overriding functions override.
• Member function reference qualifiers make it possible to treat lvalue and
rvalue objects (*this) differently.
 */

/*
***** When override keyword is not used, the signatures of both base and derived class functions must be exactly same

class Base {
public:
virtual void mf1() const;
virtual void mf2(int x);
virtual void mf3() &;
void mf4() const;
};
class Derived: public Base {
public:
virtual void mf1();
virtual void mf2(unsigned int x);
virtual void mf3() &&;
void mf4() const;
};

None of the funcs in Derived overrides funcs in Base class.
• mf1 is declared const in Base, but not in Derived.
• mf2 takes an int in Base, but an unsigned int in Derived.
• mf3 is lvalue-qualified in Base, but rvalue-qualified in Derived.
• mf4 isn’t declared virtual in Base.

*/



#include <iostream>
#include <memory>

class Base {
public:
    virtual void doWork(){
        std::cout << "Base class function called" << std::endl;
    }

    virtual void doSomething() & { // this version of doSomething applies only when *this is an lvalue
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

    void doSomething() & override { // this version of doSomething applies only when *this is an lvalue
        std::cout << "Derived class doSomething & function called" << std::endl;
    }

    void doSomething() && { // this version of doSomething applies only when *this is an rvalue
        std::cout << "Derived class doSomething && function called" << std::endl;
    }

    void func(unsigned int x) { // When this function declared with override, it gives compilation error since the signature is different with base class
        std::cout << "Derived class func is called with arg " << x << std::endl;
    }
};

Widget makeWidget() {
    return Widget();
}

int main() {

    std::unique_ptr<Base> upb = std::make_unique<Widget>();
    upb->doWork(); // Calls derived class doSomething

    Widget w;
    w.doSomething(); // Calls doSomething &

    makeWidget().doSomething(); // Calls doSomething &&

    int x = 3;
    unsigned int y = 5;

    upb->func(x); // Calls base class func
    w.func(y); // Calss derived class func

    return 0;
}