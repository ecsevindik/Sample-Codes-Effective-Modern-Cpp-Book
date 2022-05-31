
/**
• The default launch policy for std::async permits both asynchronous and
synchronous task execution.
• This flexibility leads to uncertainty when accessing thread_locals, implies
that the task may never execute, and affects program logic for timeout-based
wait calls.
• Specify std::launch::async if asynchronous task execution is essential.
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
    auto def1 = std::async(std::launch::deferred, doDeferredWork, 10);
    auto fut1 = std::async(std::launch::async, doAsyncWork, 10);

    std::cout << def1.get() << std::endl;
    std::cout << fut1.get() << std::endl;
}

void test2() {
    auto fut2 = std::async(doAsyncWork, 10);
    if(fut2.wait_for(0s) == std::future_status::deferred) {
        std::cout << fut2.get() << std::endl;
    } else {
        while(fut2.wait_for(5ms) != std::future_status::ready) {
            std::cout << "Waiting for async work to finish" << std::endl;
        }
        std::cout << fut2.get() << std::endl;
    }
}

template < typename F, typename... Ts>
inline auto
reallyAsync(F&& f, Ts&&... params) {
    return std::async(std::launch::async, 
                      std::forward<F>(f), 
                      std::forward<Ts>(params)...);
}

void test3() {
    auto fut = reallyAsync(doAsyncWork, 10);
    std::cout << "Waiting for async work to finish" << std::endl;
    std::cout << fut.get() << std::endl;
}

int main() {
    
    test1();
    test2();
    test3();

    return 0;
}