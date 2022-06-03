
/**
• Use C++14’s init capture to move objects into closures.
• In C++11, emulate init capture via hand-written classes or std::bind.
 */

#include <vector>
#include <functional>
#include <algorithm>
#include <memory>

#include "Chronometer.h"

class Widget {
public:
    Widget(bool isVal, bool isProc, bool isArch) 
    : m_IsValidated(isVal), m_IsProcessed(isProc), m_IsArchived(isArch) 
    {}

    ~Widget() = default;

    bool isValidated() const { return m_IsValidated; }
    bool isProcessed() const { return m_IsProcessed; }
    bool isArchived() const { return m_IsArchived; }

private:
    bool m_IsValidated;
    bool m_IsProcessed;
    bool m_IsArchived;
};

// Valid for C++14 or newer
void initCaptureTest() {

    std::cout << "initCaptureTest" << std::endl;

    auto pw = std::make_unique<Widget>(true, false, true);

    auto func = [pw = std::move(pw)] { return pw->isValidated() && pw->isArchived(); };
    std::cout << func() << std::endl;

    auto func2 = [pw = std::make_unique<Widget>(true, false, false)] { return pw->isValidated() && pw->isProcessed(); };
    std::cout << func2() << std::endl;

    std::cout << std::endl;
}

// C++11 way to do same thing
class IsValAndArch { // "is validated and archived"
public:
    using DataType = std::unique_ptr<Widget>;
    explicit IsValAndArch(DataType&& ptr)
    : pw(std::move(ptr)) {} // use of std::move
    
    bool operator()() const
    { return pw->isValidated() && pw->isArchived(); }
private:
    DataType pw;
};

void initCaptureCPP11Version() {
    std::cout << "initCaptureCPP11Version" << std::endl;

    auto func = IsValAndArch(std::make_unique<Widget>(true, false, true));
    std::cout << func() << std::endl;

    std::cout << std::endl;
}

// C++11 way to do same thing with bind
void bindTest() {

    std::cout << "bindTest" << std::endl;
    std::vector<double> data(3, 0.5);

    auto func1 = std::bind([](std::vector<double>& data) { // Bind object stores copies of all the arguments
            data[0] = 1.0;
        }, data);

    func1();

    auto func2 = std::bind([](std::vector<double>& data) { // Bind object stores reference now
            data[1] = 1.0;
        }, std::ref(data));

    func2();

    auto func3 = std::bind([](const std::vector<double>& data) { // C++11 emulation of init capture
            for(auto& d : data) {
                std::cout << d << " - ";
            }
            std::cout << std::endl;
        }, std::move(data));

    func3();

    std::cout << std::endl;
}

int main() {
    initCaptureTest();
    initCaptureCPP11Version();
    bindTest();
    return 0;
}