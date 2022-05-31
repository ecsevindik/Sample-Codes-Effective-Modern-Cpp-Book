
/**
• Use std::weak_ptr for std::shared_ptr-like pointers that can dangle.
• Potential use cases for std::weak_ptr include caching, observer lists, and the
prevention of std::shared_ptr cycles.
 */

#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;

class Widget {
public:
    Widget(int id) : m_id(id) {}
    ~Widget() { std::cout << "Widget " << m_id << " destructor called" << std::endl;};

private:
    int m_id;
};

void test1() {
    auto spw =std::make_shared<Widget>(3); // after spw is constructed, the pointed-to Widget's ref count (RC) is 1.

    std::weak_ptr<Widget> wpw(spw); // wpw points to same Widget  as spw. RC remains 1

    spw = nullptr; // RC goes to 0, and the Widget is destroyed. wpw now dangles

    if (wpw.expired()) {
        std::cout << "spw dangles" << std::endl;
    }
}

void test2() {
    auto spw1 =std::make_shared<Widget>(3);

    std::weak_ptr<Widget> wpw(spw1);

    spw1 = nullptr;

    auto spw2 = wpw.lock(); // If wpw1 has expired, spw1 is null

    try {
        std::shared_ptr<Widget> spw3(wpw); // Expected to throw bad_weak_ptr
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

std::shared_ptr<const Widget> loadWidget(int id) {
    return make_shared<const Widget>(id);
}

// Quick and dirty implementation for caching Widget ptrs
std::shared_ptr<const Widget> fastLoadWidget(int id) {
    static std::unordered_map<int, std::weak_ptr<const Widget>> cache;

    auto objPtr = cache[id].lock();

    if(!objPtr) {
        objPtr = loadWidget(id);
        cache[id] = objPtr;
    }

    return objPtr;
}

void test3() {
    auto spw1 = fastLoadWidget(4);
    auto spw2 = fastLoadWidget(5);
    auto spw3 = fastLoadWidget(5); // Destructor of 5 is called only once since it is cached
}

int main() {
    test1();
    test2();
    test3();
    return 0;
}