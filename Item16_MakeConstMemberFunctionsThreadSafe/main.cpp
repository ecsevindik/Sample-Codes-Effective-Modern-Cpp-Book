
/*
• Make const member functions thread safe unless you’re certain they’ll never
be used in a concurrent context.
• Use of std::atomic variables may offer better performance than a mutex, but
they’re suited for manipulation of only a single variable or memory location.
 */

#include <thread>
#include <mutex>

#include "Chronometer.h"

using namespace std::chrono_literals;

int expensiveComputation() {
    std::this_thread::sleep_for(100ms);
    return 1;
}

class Widget {
public:
    Widget() = default;
    int magicValue() const // const function
    {
        std::lock_guard<std::mutex> guard(m); // lock m
        if (cacheValid) return cachedValue;
        else {
            auto val1 = expensiveComputation();
            auto val2 = expensiveComputation();
            cachedValue = val1 + val2;
            cacheValid = true;
            return cachedValue;
        }
    } // unlock m

private:
    mutable std::mutex m; // mutable keyword helps to change member variables in a const member functions
    mutable int cachedValue; // no need to be atomic
    mutable bool cacheValid{ false }; // no need to be atomic
};

int main() {

    utils::Chronometer ch;
    Widget w;
    ch.Start();
    w.magicValue();
    ch.Stop();
    ch.Report("First magicValue call ");

    std::thread th([&](){
            utils::Chronometer ch;
            ch.Start();
            w.magicValue();
            ch.Stop();
            ch.Report("Paralel magicValue call ");
        });

    ch.Start();
    w.magicValue();
    ch.Stop();
    ch.Report("Second magicValue call ");
    
    th.join();

    return 0;
}