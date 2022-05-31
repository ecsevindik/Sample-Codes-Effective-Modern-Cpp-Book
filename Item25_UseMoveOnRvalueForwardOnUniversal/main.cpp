
/*
• Apply std::move to rvalue references and std::forward to universal references
the last time each is used.
• Do the same thing for rvalue references and universal references being
returned from functions that return by value.
• Never apply std::move or std::forward to local objects if they would otherwise
be eligible for the return value optimization.
 */

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class DummyClass {
public:
    DummyClass() =default;
    ~DummyClass() = default;

    DummyClass(DummyClass&& rhs)
    : m_x(std::move(rhs.m_x)) { 
        std::cout << "DummyClass move constructor called" << std::endl;
    }

    DummyClass& operator=(DummyClass&& rhs)
    { 
        m_x = std::move(rhs.m_x);
        std::cout << "DummyClass move assigment called" << std::endl;
        return *this;
    }

    DummyClass(const DummyClass& rhs) {
        m_x = rhs.m_x;
        std::cout << "DummyClass copy constructor called" << std::endl;
    }

    DummyClass& operator=(const DummyClass& rhs)
    { 
        m_x = rhs.m_x;
        std::cout << "DummyClass copy assigment called" << std::endl;
        return *this;
    }

private:
    int m_x;
};

class Widget {
public:
    Widget() = default;
    ~Widget() = default;

    template<typename T>
    void setClass(T&& cls) {
        m_dumClass = std::forward<T>(cls);
    }

    void setString(std::string&& str) {
        m_dumString = std::move(str);
    }

private:
    DummyClass m_dumClass;
    std::string m_dumString;
};

int main() {

    DummyClass cls;
    Widget w1;
    w1.setClass(cls);
    Widget w2;
    w2.setClass(std::move(cls));

    std::string str = "Bart";
    w1.setString(std::move(str));
    std::cout << str << std::endl; // Expected to console out empty or throw exception

    return 0;
}