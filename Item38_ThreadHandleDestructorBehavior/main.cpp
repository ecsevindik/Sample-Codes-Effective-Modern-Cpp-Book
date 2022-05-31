
/**
• Future destructors normally just destroy the future’s data members.
• The final future referring to a shared state for a non-deferred task launched
via std::async blocks until the task completes.
 */

#include <future>

#include "Chronometer.h"

using namespace std::literals;

int doAsyncWork(int sleepms) {
    std::cout << "Async work performed - " << sleepms << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds{sleepms});
    return 10;
}

int doDeferredWork(int sleepms) {
    std::cout << "Deferred work performed - " << sleepms << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds{sleepms});
    return 5;
}

void test1() {
    std::shared_future<int> shared1 = std::async(std::launch::async, doAsyncWork, 10);
    {
        auto shared2 = shared1;
    }
    std::cout << "First scope exited" << std::endl;

    {
        std::future<int> fut1 = std::async(std::launch::async, doAsyncWork, 20);
    }
    std::cout << "Second scope exited" << std::endl;

}

int main() {
    
    test1();
    return 0;
}