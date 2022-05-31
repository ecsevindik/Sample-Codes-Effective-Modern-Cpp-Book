
/**
• std::unique_ptr is a small, fast, move-only smart pointer for managing
resources with exclusive-ownership semantics.
• By default, resource destruction takes place via delete, but custom deleters
can be specified. Stateful deleters and function pointers as deleters increase the
size of std::unique_ptr objects.
• Converting a std::unique_ptr to a std::shared_ptr is easy.
 */


#include <string>
#include <iostream>
#include <memory>

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
auto makeInvestmentWithCustomDel(InvestmentType type, Ts&&... params) {

    std::unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt); // ptr to be returned

    if(type == InvestmentType::Stock) {
        pInv.reset(new Stock(std::forward<Ts>(params)...));
    } else if (type == InvestmentType::Bond) {
        pInv.reset(new Bond(std::forward<Ts>(params)...));
    } else if (type == InvestmentType::RealEstate) {
        pInv.reset(new RealEstate(std::forward<Ts>(params)...));
    }
    return pInv;
}

template< typename... Ts>
std::unique_ptr<Investment> makeInvestment(InvestmentType type, Ts&&... params) {

    if(type == InvestmentType::Stock) {
        return make_unique<Stock>(std::forward<Ts>(params)...);
    } else if (type == InvestmentType::Bond) {
        return make_unique<Bond>(std::forward<Ts>(params)...);
    } else if (type == InvestmentType::RealEstate) {
        return make_unique<RealEstate>(std::forward<Ts>(params)...);
    }
    return nullptr;
}

int main() {

    auto invt1 = makeInvestmentWithCustomDel(InvestmentType::Bond, 3);
    {
        auto invt2 = makeInvestmentWithCustomDel(InvestmentType::Stock, 4);
    }

    // Convert unique_ptr to shared_ptr
    std::shared_ptr<Investment> sp = makeInvestmentWithCustomDel(InvestmentType::RealEstate, 5);

    auto invt3 = makeInvestment(InvestmentType::Bond, 3);

    return 0;
}