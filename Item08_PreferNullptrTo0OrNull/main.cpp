
/**
• Prefer nullptr to 0 and NULL.
• Avoid overloading on integral and pointer types.
 */

#include <mutex>
#include <memory>

using MuxGuard = std::lock_guard<std::mutex>;

class Widget {
public:
    Widget() = default;
};

int f1(std::shared_ptr<Widget> spw) { return 1;} // call these only when
double f2(std::unique_ptr<Widget> upw) { return 2.0;} // the appropriate
bool f3(Widget* pw) { return false;} // mutex is locked

void test1() {
    std::mutex f1m, f2m, f3m; // mutexes for f1, f2, and f3
    
    {
        MuxGuard g(f1m); // lock mutex for f1
        f1(0); // pass 0 as null ptr to f1
    } // unlock mutex

    {
        MuxGuard g(f2m); // lock mutex for f2
        f2(NULL); // pass NULL as null ptr to f2
    } // unlock mutex

    {
        MuxGuard g(f3m); // lock mutex for f3
        f3(nullptr); // pass nullptr as null ptr to f3
    } // unlock mutex
}

template<typename FuncType,
typename MuxType,
typename PtrType>
decltype(auto) lockAndCall(FuncType func,
                 MuxType& mutex,
                 PtrType ptr)
{
    MuxGuard g(mutex);
    return func(ptr);
}

void test2() {
    std::mutex f1m, f2m, f3m; // mutexes for f1, f2, and f3

    // auto result1 = lockAndCall(f1, f1m, 0); // error since 0 is deduced as integral type int. uncomment to see error
    // auto result2 = lockAndCall(f2, f2m, NULL); // error since 0 is deduced as integral type int. uncomment to see error
    lockAndCall(f3, f3m, nullptr); // fine
}

int main() {
    test1();
    test2();
    return 0;
}