
/*
• Perfect forwarding fails when template type deduction fails or when it deduces
the wrong type.
• The kinds of arguments that lead to perfect forwarding failure are braced initializers,
null pointers expressed as 0 or NULL, declaration-only integral const
static data members, template and overloaded function names, and bitfields.
 */

#include <iostream>
#include <vector>

void f1(const std::vector<int>& v) {
  for(auto& vv : v) {
    std::cout << vv << std::endl;
  }
}

template< typename ... Ts>
void fwd1(Ts&&... params) {
  f1(std::forward<Ts>(params)...);
}

// Braced initializers
void bracedInitializerTest(){
  std::cout << "bracedInitializerTest" << std::endl; 
  f1({1,2,3});
  // fwd1({1,2,3}); // Does not compiles since compiler cannot deduce correct type from {1,2,3}

  auto vec = {4,5,6}; // auto deduce the type as std::initializer_list<int>
  fwd1(vec); // Compiles since type is explicitly defined

  std::cout << std::endl;
}

class Widget {
public:
  static const std::size_t MinVals = 28;

  Widget() = default;
  ~Widget() = default;
};

void f2(std::size_t val){
  std::cout << val << std::endl;
}

template< typename ... Ts>
void fwd2(Ts&&... params) {
  f2(std::forward<Ts>(params)...);
}

void declarationOnlyStaticConstDataMemberTest() {

  std::cout << "declarationOnlyStaticConstDataMemberTest" << std::endl;

  f2(Widget::MinVals); // Compilers work around the missing definition by plopping the value 28 into all places where MinVals is mentioned.

  // fwd2(Widget::MinVals); // With gcc compiler, this does not link since it requires references to MinVals which is not defined here. However, it works with Clang compiler.
  
  std::cout << std::endl;
}

void f3(int pf(int)){
  std::cout << pf(5) << std::endl;
}

int processVal(int value) {return value;}
int processVal(int value, int priority) {return value*priority;}

template< typename ... Ts>
void fwd3(Ts&&... params) {
  f3(std::forward<Ts>(params)...);
}

void functionOverloadingTest(){

  std::cout << "functionOverloadingTest" << std::endl;

  f3(processVal);
  // fwd3(processVal); // Does not compile since it does not know which overloaded processVal function to use.

  // Workaround
  using ProcessFuncType = int (*)(int);
  ProcessFuncType processValPtr = processVal;
  fwd3(processValPtr); // fine
  fwd3(static_cast<ProcessFuncType>(processVal)); // also fine

  std::cout << std::endl;
}

struct IPv4Header {
std::uint32_t version:4,
              IHL:4,
              DSCP:6,
              ECN:2,
              totalLength:16;
};

void bitfieldsTest(){
  std::cout << "bitfieldsTest" << std::endl;

  IPv4Header h;
  h.totalLength =3;

  f2(h.totalLength);

  // fwd2(h.totalLength); // Does not compile since bitfields do not have references, it is forbidden

  // Workaround
  auto length = static_cast<std::uint16_t>(h.totalLength);
  fwd2(length);

  std::cout << std::endl;
}

int main() {

    bracedInitializerTest();
    declarationOnlyStaticConstDataMemberTest();
    functionOverloadingTest();
    bitfieldsTest();
    return 0;
}