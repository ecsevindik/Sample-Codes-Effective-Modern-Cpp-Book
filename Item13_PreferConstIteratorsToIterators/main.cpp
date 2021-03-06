
/*
• Prefer const_iterators to iterators.
• In maximally generic code, prefer non-member versions of begin, end,
rbegin, etc., over their member function counterparts.
 */


#include <iostream>
#include <vector>
#include <algorithm>

namespace ecs {
    // C++11 does not have non-member cbegin function. It can be written this way.
    // std::begin() returns const pointer for const C
    template <class C>
    auto cbegin(const C& container)->decltype(std::begin(container))
    {
        return std::begin(container);
    }
}

void findAndInsertTest() {
    std::vector<int> values{1, 2100, 1983, 3, 2010};

    auto it = std::find(values.cbegin(), values.cend(), 1983);
    values.insert(it, 1998);

    for(auto& val : values) {
        std::cout << val << "-";
    }
    std::cout << std::endl;
}

template<typename C, typename V>
void findAndInsert( C& container, // in container, find first occurrence of targetVal, then insert insertVal there
                    const V& targetVal,
                    const V& insertVal)
{
    using std::cbegin;
    using std::cend;
    auto it = std::find(cbegin(container), // non-member cbegin
                        cend(container), // non-member cend
                        targetVal);
    container.insert(it, insertVal);
}

void templatizedFindAndInsertTest() {
    std::vector<int> values{1, 2100, 3, 2010, 1983};
    findAndInsert(values, 1983, 1998);

    for(auto& val : values) {
        std::cout << val << "-";
    }
    std::cout << std::endl;
}

int main() {
    findAndInsertTest();
    templatizedFindAndInsertTest();
    return 0;
}
