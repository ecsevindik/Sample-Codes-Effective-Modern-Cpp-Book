
/**
• std::shared_ptrs offer convenience approaching that of garbage collection
for the shared lifetime management of arbitrary resources.
• Compared to std::unique_ptr, std::shared_ptr objects are typically
twice as big, incur overhead for control blocks, and require atomic reference
count manipulations.
• Default resource destruction is via delete, but custom deleters are supported.
The type of the deleter has no effect on the type of the std::shared_ptr.
• Avoid creating std::shared_ptrs from variables of raw pointer type.
 */


#include <string>
#include <iostream>
#include <vector>

using namespace std;

enum class InvestmentType {Stock, Bond, RealEstate};

class Investment {
public:
    Investment(int x) : m_x(x) {};
    virtual ~Investment() = default;
    int m_x;
};

class Stock : public Investment {
public:
    Stock(int x) : Investment(x) {}
};

class Bond : public Investment {
public:
    Bond(int x) : Investment(x) {}
};

class RealEstate : public Investment {
public:
    RealEstate(int x) :Investment(x) {}
};

// Custom deleter
auto delInvmt = [] (Investment* pInvestment) {
    std::cout << "Deleting " << pInvestment->m_x << std::endl;
    delete pInvestment;
};

template< typename... Ts>
std::shared_ptr<Investment> makeInvestmentWithCustomDel(InvestmentType type, Ts&&... params) {

    if(type == InvestmentType::Stock) {
        return std::shared_ptr<Investment>(new Stock(std::forward<Ts>(params)...), delInvmt);
    } else if (type == InvestmentType::Bond) {
        return std::shared_ptr<Investment>(new Bond(std::forward<Ts>(params)...), delInvmt);
    } else if (type == InvestmentType::RealEstate) {
        return std::shared_ptr<Investment>(new RealEstate(std::forward<Ts>(params)...), delInvmt);
    }
    return nullptr;
}

template< typename... Ts>
std::shared_ptr<Investment> makeInvestment(InvestmentType type, Ts&&... params) {

    if(type == InvestmentType::Stock) {
        return make_shared<Stock>(std::forward<Ts>(params)...);
    } else if (type == InvestmentType::Bond) {
        return make_shared<Bond>(std::forward<Ts>(params)...);
    } else if (type == InvestmentType::RealEstate) {
        return make_shared<RealEstate>(std::forward<Ts>(params)...);
    }
    return nullptr;
}

void test1() {
    auto invt1 = makeInvestmentWithCustomDel(InvestmentType::Bond, 3);
    {
        auto invt2 = makeInvestmentWithCustomDel(InvestmentType::Stock, 4);
        auto invt3 = invt2;
    }

    auto invt4 = makeInvestment(InvestmentType::Bond, 3);
}

void test2() {

    auto customDel1 = [](Investment* inv) {
        std::cout << "Custom del 1  -  " << inv->m_x << std::endl;
        delete inv;
    };

    auto customDel2 = [](Investment* inv) {
        std::cout << "Custom del 2  -  " << inv->m_x << std::endl;
        delete inv;
    };

    std::shared_ptr<Investment> pw1 = std::shared_ptr<Investment>(new Bond(10), customDel1);
    std::shared_ptr<Investment> pw2 = std::shared_ptr<Investment>(new Stock(20), customDel2);

    std::vector<std::shared_ptr<Investment>> vpw;
    vpw.emplace_back(std::move(pw1));
    vpw.emplace_back(std::move(pw2));
    vpw[0] = vpw[1]; // shared ptr in index 0 deleted here
}

int main() {

    test1();
    test2();
    
    return 0;
}