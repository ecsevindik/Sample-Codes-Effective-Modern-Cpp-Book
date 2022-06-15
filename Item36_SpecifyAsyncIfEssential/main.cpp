
/*
• The default launch policy for std::async permits both asynchronous and
synchronous task execution.
• This flexibility leads to uncertainty when accessing thread_locals, implies
that the task may never execute, and affects program logic for timeout-based
wait calls.
• Specify std::launch::async if asynchronous task execution is essential.
 */

#include <iostream>
#include <future>

using namespace std::literals;

int doAsyncWork(int sleepms) noexcept {
    std::cout << "Async work performing - " << sleepms << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds{sleepms});
    return 10;
}

int doDeferredWork(int sleepms) noexcept {
    std::cout << "Deferred work performing - " << sleepms << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds{sleepms});
    return 5;
}

void launchPolicyTest() {
    std::cout << "launchPolicyTest" << std::endl;
    auto def1 = std::async(std::launch::deferred, doDeferredWork, 10);
    auto fut1 = std::async(std::launch::async, doAsyncWork, 10);

    std::cout << def1.get() << std::endl;
    std::cout << fut1.get() << std::endl;

    std::cout << std::endl;
}

void statusTest() {
    std::cout << "statusTest" << std::endl;

    auto fut2 = std::async(doAsyncWork, 10);
    if(fut2.wait_for(0s) == std::future_status::deferred) { // get status of async work from future
        std::cout << fut2.get() << std::endl;
    } else {
        while(fut2.wait_for(5ms) != std::future_status::ready) {
            std::cout << "Waiting for async work to finish" << std::endl;
        }
        std::cout << fut2.get() << std::endl;
    }

    std::cout << std::endl;
}

template < typename F, typename... Ts>
inline auto
reallyAsync(F&& f, Ts&&... params) {
    return std::async(std::launch::async, 
                      std::forward<F>(f), 
                      std::forward<Ts>(params)...);
}

void templatizedAsyncCallTest() {
    std::cout << "templatizedAsyncCallTest" << std::endl;
    auto fut = reallyAsync(doAsyncWork, 10);
    std::cout << "Waiting for async work to finish" << std::endl;
    std::cout << fut.get() << std::endl;
    std::cout << std::endl;

    auto fut2 = reallyAsync([](int x){std::cout << x << std::endl;}, 3);
}

int main() {
    
    launchPolicyTest();
    statusTest();
    templatizedAsyncCallTest();

    return 0;
}
