
/**
• For copyable, cheap-to-move parameters that are always copied, pass by value
may be nearly as efficient as pass by reference, it’s easier to implement, and it
can generate less object code.
• Copying parameters via construction may be significantly more expensive
than copying them via assignment.
• Pass by value is subject to the slicing problem, so it’s typically inappropriate
for base class parameter types.
 */

#include <vector>

#include "Chronometer.h"

class Approach1 {
    public:
    void addName(const std::string& newName) {
        names.push_back(newName);
    }

    void addName(std::string&& newName) {
        names.push_back(std::move(newName));
    }

    private:
    std::vector<std::string> names;
};

class Approach2 {
    public:
    template<typename T>
    void addName(T&& newName) {
        names.push_back(std::forward<T>(newName));
    }

    private:
    std::vector<std::string> names;
};

class Approach3 {
    public:
    void addName(std::string newName) {
        names.push_back(std::move(newName));
    }

    private:
    std::vector<std::string> names;
};

int test() {

    utils::Chronometer chronometer;

    Approach1 dum1;

    Approach1 app1;
    Approach2 app2;
    Approach3 app3;

    std::string name = "Bart";

    // Warm=up
    for(int i = 0; i < 100; ++i) {
        dum1.addName(name);
    }

    // Approach1
    chronometer.Start();
    for(int i = 0; i < 500; ++i) {
        app1.addName(name);
        app1.addName(name + " Jenne");
    }
    chronometer.Stop();
    chronometer.Report("Approach1 ");

    // Approach2
    chronometer.Start();
    for(int i = 0; i < 500; ++i) {
        app2.addName(name);
        app2.addName(name + " Jenne");
    }
    chronometer.Stop();
    chronometer.Report("Approach2 ");

    // Approach3
    chronometer.Start();
    for(int i = 0; i < 500; ++i) {
        app3.addName(name);
        app3.addName(name + " Jenne");
    }
    chronometer.Stop();
    chronometer.Report("Approach3 ");

    std::cout << std::endl;

    return 0;
}

int main() {
    for(int i = 0; i < 10; ++i) {
        test();
    }
}