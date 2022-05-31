
/**
• auto variables must be initialized, are generally immune to type mismatches
that can lead to portability or efficiency problems, can ease the process of
refactoring, and typically require less typing than variables with explicitly
specified types.
• auto-typed variables are subject to the pitfalls described in Items 2 and 6.
 */

/*
// auto helps reducing the coding effort
    template<typename It> // algorithm to dwim ("do what I mean")
    void dwim(It b, It e) // for all elements in range from
    {                     // b to e
        while (b != e) {
        typename std::iterator_traits<It>::value_type // auto can be used instead
        currValue = *b;
        …

        }
    }

    auto derefUPLess = // comparison func. for Widgets pointed to by std::unique_ptrs
        [](const std::unique_ptr<Widget>& p1,
        const std::unique_ptr<Widget>& p2)
        { return *p1 < *p2; }; 

    auto derefLess = // C++14 comparison function for values pointed to by anything pointer-like
        [](const auto& p1,
        const auto& p2)
        { return *p1 < *p2; };

    Without auto:
    std::function<bool( const std::unique_ptr<Widget>&,
                        const std::unique_ptr<Widget>&)>
        derefUPLess = [](const std::unique_ptr<Widget>& p1,
                         const std::unique_ptr<Widget>& p2)
                         { return *p1 < *p2; };
    
    // **** The std::function approach is generally bigger and slower than the auto approach.
*/

#include <iostream>
#include <vector>
#include <unordered_map>

int main() {

    // auto prevents uninitialized variable
    int x1; // potentially uninitialized
    // auto x2; // error! initializer required
    auto x3 = 0; // fine, x's value is well-defined

    std::unordered_map<std::string, int> m;
    m.emplace("abc", 123);
    m.emplace("def", 456);

    
    // for(std::pair<std::string, int>& p : m) {} // Error since each pair in m has a type of std::pair<const std::string, int>
    for(const std::pair<std::string, int>& p : m) { // This causes compiler to create reference to a temp object by copying from std::pair<const std::string, int> to const std::pair<std::string, int>
        std::cout << p.second << std::endl; // That temp object will be out of scope at the end
    }

    for(auto& p: m) { // using auto prevents such problems
        p.second = 3;
    }

     for(auto& p: m) {
        std::cout << p.second << std::endl;
    }

    return 0;
}