
/*
• For simple event communication, condvar-based designs require a superfluous
mutex, impose constraints on the relative progress of detecting and reacting
tasks, and require reacting tasks to verify that the event has taken place.
• Designs employing a flag avoid those problems, but are based on polling, not
blocking.
• A condvar and flag can be used together, but the resulting communications
mechanism is somewhat stilted.
• Using std::promises and futures dodges these issues, but the approach uses
heap memory for shared states, and it’s limited to one-shot communication.
 */

#include <iostream>
#include <future>

using namespace std::literals;

// taken from cppreference https://en.cppreference.com/w/cpp/thread/shared_future
int main() {
    
    std::promise<void> ready_promise, t1_ready_promise, t2_ready_promise;
    std::shared_future<void> ready_future(ready_promise.get_future());
 
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
 
    auto fun1 = [&, ready_future]() -> std::chrono::duration<double, std::milli> 
    {
        t1_ready_promise.set_value();
        ready_future.wait(); // waits for the signal from main()
        return std::chrono::high_resolution_clock::now() - start;
    };
 
 
    auto fun2 = [&, ready_future]() -> std::chrono::duration<double, std::milli> 
    {
        t2_ready_promise.set_value();
        ready_future.wait(); // waits for the signal from main()
        return std::chrono::high_resolution_clock::now() - start;
    };
 
    auto fut1 = t1_ready_promise.get_future();
    auto fut2 = t2_ready_promise.get_future();
 
    auto result1 = std::async(std::launch::async, fun1);
    auto result2 = std::async(std::launch::async, fun2);
 
    // wait for the threads to become ready
    fut1.wait();
    fut2.wait();
 
    // the threads are ready, start the clock
    start = std::chrono::high_resolution_clock::now();
 
    // signal the threads to go
    ready_promise.set_value();
 
    std::cout << "Thread 1 received the signal "
              << result1.get().count() << " ms after start\n"
              << "Thread 2 received the signal "
              << result2.get().count() << " ms after start\n";
    return 0;
}
