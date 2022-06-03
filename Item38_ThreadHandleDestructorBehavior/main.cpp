
/*
• Future destructors normally just destroy the future’s data members.
• The final future referring to a shared state for a non-deferred task launched
via std::async blocks until the task completes.
 */

#include <future>

#include "Chronometer.h"

using namespace std::literals;

int doAsyncWork(int sleepms) {
    std::cout << "Async work performing - " << sleepms << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds{sleepms});
    return 10;
}

int doDeferredWork(int sleepms) {
    std::cout << "Deferred work performing - " << sleepms << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds{sleepms});
    return 5;
}

void sharedStateTest() {
    std::cout << "sharedStateTest" << std::endl;

    std::shared_future<int> shared1 = std::async(std::launch::async, doAsyncWork, 10);
    {
        auto shared2 = shared1;
    } // shared2 is destructed here, however, async work does not block the main thread since there is another shared state pointing to async's promise.
    std::cout << "First scope exited" << std::endl;

    {
        std::future<int> fut1 = std::async(std::launch::async, doAsyncWork, 20);
    }// since fut1 is destructed here, code is blocked to wait for async work to complete since there is no other future waiting for async's work
    std::cout << "Second scope exited" << std::endl;

    std::cout << std::endl;

}

// Dummy function
int calcValue () { return 3; }

/*
The class template std::packaged_task wraps any Callable target 
(function, lambda expression, bind expression, or another function object) 
so that it can be invoked asynchronously. Its return value or exception thrown 
is stored in a shared state which can be accessed through std::future objects. 
*/
void packagedTaskTest() {
    std::cout << "packagedTaskTest" << std::endl;

    std::packaged_task<int()> pt(calcValue);

    auto fut = pt.get_future();

    std::thread t(std::move(pt));

    t.join(); // Blocks this thread until t finishes. Otherwise, t becomes joinable at the end of the scope and program is terminated.

    std::cout << fut.get() << std::endl;

    std::cout << std::endl;
}

int main() {
    
    sharedStateTest();
    packagedTaskTest();
    return 0;
}