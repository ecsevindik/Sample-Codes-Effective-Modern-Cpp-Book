
/**
• The std::thread API offers no direct way to get return values from asynchronously
run functions, and if those functions throw, the program is terminated.
• Thread-based programming calls for manual management of thread exhaustion,
oversubscription, load balancing, and adaptation to new platforms.
• Task-based programming via std::async with the default launch policy handles
most of these issues for you.
 */

#include <future>

#include "Chronometer.h"

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

int main() {
    auto fut = std::async(doAsyncWork, 40);
    auto fut2 = std::async(doAsyncWork, 20);
    std::cout << fut2.get() << std::endl;

    auto def1 = std::async(std::launch::deferred, doDeferredWork, 10);
    auto def2 = std::async(std::launch::deferred, doDeferredWork, 20);

    std::cout << def2.get() << std::endl;
    std::cout << def1.get() << std::endl;

    return 0;
}