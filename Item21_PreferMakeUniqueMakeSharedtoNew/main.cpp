
/**
• Compared to direct use of new, make functions eliminate source code duplication,
improve exception safety, and, for std::make_shared and std::allo
cate_shared, generate code that’s smaller and faster.
• Situations where use of make functions is inappropriate include the need to
specify custom deleters and a desire to pass braced initializers.
• For std::shared_ptrs, additional situations where make functions may be
ill-advised include (1) classes with custom memory management and (2) systems
with memory concerns, very large objects, and std::weak_ptrs that
outlive the corresponding std::shared_ptrs.
 */

/* Explanation

processWidget(std::shared_ptr<Widget>(new Widget),  // potential
                                computePriority()); // resource
                                                    // leak!

That is, compilers may emit code to execute the operations in this order:
1. Perform “new Widget”.
2. Execute computePriority.
3. Run std::shared_ptr constructor.
If such code is generated and, at runtime, computePriority produces an exception,
the dynamically allocated Widget from Step 1 will be leaked, because it will never be
stored in the std::shared_ptr that’s supposed to start managing it in Step 3.

processWidget(std::make_shared<Widget>(),   // no potential
                        computePriority()); // resource leak

* Drawback of make_***
As long as std::weak_ptrs refer to a control block (i.e., the weak count is greater
than zero), that control block must continue to exist. And as long as a control block
exists, the memory containing it must remain allocated. The memory allocated by a
std::shared_ptr make function, then, can’t be deallocated until the last
std::shared_ptr and the last std::weak_ptr referring to it have been destroyed

*/

#include <vector>

#include "Chronometer.h"

class Widget {
public:
    Widget(int id) : m_id(id) {}
    ~Widget() = default;

private:
    int m_id;
};

void test1() {
    std::vector<std::shared_ptr<Widget>> vec1;

    utils::Chronometer ch;
    ch.Start();
    for(int i = 0; i < 1000; ++i) {
        vec1.emplace_back(std::make_shared<Widget>(3));
    }
    ch.Stop();
    ch.Report("Make shared - ");

    ch.Start();
    for(int i = 0; i < 1000; ++i) {
        vec1.emplace_back(std::shared_ptr<Widget>(new Widget(3)));
    }
    ch.Stop();
    ch.Report("From raw pointer - ");
}

int main() {
    test1();
    return 0;
}