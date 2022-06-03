
/*
• Assume that move operations are not present, not cheap, and not used.
• In code with known types or support for move semantics, there is no need for
assumptions.
 */

#include <vector>
#include <array>
#include "Chronometer.h"

using namespace utils;

/*
There are thus several scenarios in which C++11’s move semantics do you no good:
  • No move operations: The object to be moved from fails to offer move opera‐
  tions. The move request therefore becomes a copy request.
  • Move not faster: The object to be moved from has move operations that are no
  faster than its copy operations.
  • Move not usable: The context in which the moving would take place requires a
  move operation that emits no exceptions, but that operation isn’t declared noex
  cept.
  
It’s worth mentioning, too, another scenario where move semantics offers no effi‐
ciency gain:
  • Source object is lvalue: With very few exceptions (see e.g., Item 25) only rvalues
  may be used as the source of a move operation.
*/

/*

* Widget class has no move constructor, therefore copy ctor is used when it is moved.
* WidgetNoExcept class has noexcept move ctor, which makes moving it much faster

*/

class Widget {
public:
  Widget() = default;
  ~Widget() = default;

  Widget(Widget&& rhs)  = delete;

  Widget(const Widget& rhs)
  : x(rhs.x), y(rhs.y), s(rhs.s) {}

private:
  int x = 5;
  double y = 10.;
  std::string s = "ASHFAKSDNFJASDNASHDJASDHASDJK";
};

class WidgetNoExcept {
public:
  WidgetNoExcept() = default;
  ~WidgetNoExcept() = default;

  WidgetNoExcept(WidgetNoExcept&& rhs) noexcept
  : x(std::move(rhs.x)), y(std::move(rhs.y)), s(std::move(rhs.s)) {}

  WidgetNoExcept(const WidgetNoExcept& rhs) noexcept
  : x(rhs.x), y(rhs.y), s(rhs.s) {}

private:
  int x = 1;
  double y = 2.;
  std::string s = "ASHFAKSDNFJASDNASHDJASDHASDJK";
};

int main() {
    Chronometer ch;

    // warm up
    std::array<Widget, 100> vecdum;

    ch.Start();
    std::array<Widget, 1000> vec1;
    ch.Stop();
    ch.Report("Widget allocation ");

    ch.Start();
    std::array<WidgetNoExcept, 1000> vec2;
    ch.Stop();
    ch.Report("WidgetNoExcept allocation ");

    ch.Start();
    auto vec1Move(std::move(vec1));
    ch.Stop();
    ch.Report("Widget move ");

    ch.Start();
    auto vec2Move(std::move(vec2));
    ch.Stop();
    ch.Report("WidgetNoExcept move ");

    return 0;
}
