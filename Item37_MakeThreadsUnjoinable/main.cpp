
/**
• Make std::threads unjoinable on all paths.
• join-on-destruction can lead to difficult-to-debug performance anomalies.
• detach-on-destruction can lead to difficult-to-debug undefined behavior.
• Declare std::thread objects last in lists of data members.
 */

#include <future>

#include "Chronometer.h"

using namespace std::literals;

class ThreadRAII {
public:
    enum class DtorAction {join, detach} ;

    ThreadRAII(std::thread&& t, DtorAction a) 
    : action(a), t(std::move(t)) {}

    ~ThreadRAII() {
        if(t.joinable()) {
            if(action == DtorAction::join) {
                t.join();
                std::cout << "Thread is joined" << std::endl;
            } else {
                t.detach();
            }
        }
    }

    ThreadRAII(ThreadRAII&&) = default; // support moving
    ThreadRAII& operator=(ThreadRAII&&) = default;

    std::thread& get() {return t;}
    
private:
    DtorAction action;
    std::thread t;
};

constexpr auto tenMillion = 10'000'000;

bool doWork(std::function<bool(int)> filter, int maxVal = tenMillion, bool sleep = false) {
    std::vector<int> goodVals;
    std::atomic<bool> flag{false};
    ThreadRAII t(
        std::thread([&filter, maxVal, &goodVals, &flag] {
            for(auto i = 0; i < maxVal; ++i) {
                if(filter(i)) {
                    goodVals.push_back(i);
                }
            }
            flag = true;
        } ),
        ThreadRAII::DtorAction::join
    );

    auto nh = t.get().native_handle();

    if(sleep)
        std::this_thread::sleep_for(100ms);
    
    if(flag == true) {
        t.get().join();
        std::cout << goodVals.size() << std::endl;
        return true;
    }

    return false;
}

bool filter(int i) {
    if(i > 30)
        return false;
    else
        return true;
}

int main() {
    
    bool res = doWork(filter, 1000);
    std::cout << res << std::endl;

    bool res2 = doWork(filter, 1000, true);
    std::cout << res2 << std::endl;
    return 0;
}