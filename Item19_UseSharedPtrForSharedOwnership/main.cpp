
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

void customDelTest() {
    auto invt1 = makeInvestmentWithCustomDel(InvestmentType::Bond, 3);
    {
        auto invt2 = makeInvestmentWithCustomDel(InvestmentType::Stock, 4);
        auto invt3 = invt2;
    }

    auto invt4 = makeInvestment(InvestmentType::Bond, 5);
}

void seperateCustomDelTest() {

    auto customDel1 = [](Investment* inv) {
        std::cout << "Custom del 1  -  " << inv->m_x << std::endl;
        delete inv;
    };

    auto customDel2 = [](Investment* inv) {
        std::cout << "Custom del 2  -  " << inv->m_x << std::endl;
        delete inv;
    };

    auto pw1 = std::shared_ptr<Investment>(new Bond(10), customDel1);
    auto pw2 = std::shared_ptr<Investment>(new Stock(20), customDel2);

    std::vector<std::shared_ptr<Investment>> vpw;
    vpw.emplace_back(std::move(pw1));
    vpw.emplace_back(std::move(pw2));
    vpw[0] = vpw[1]; // shared ptr in index 0 deleted here
}

int main() {

    customDelTest();
    seperateCustomDelTest();
    
    return 0;
}

/* std::enable_shared_from_this<T> explanation

std::vector<std::shared_ptr<Widget>> processedWidgets;

class Widget {
public:
    …
    void process();
    …
};

void Widget::process() // process the Widget add it to list of processed Widgets; this is wrong!
{
    …
    processedWidgets.emplace_back(this);
}


The std::shared_ptr thus constructed will create a new control block for the pointed-to Widget (*this). 
That doesn’t sound harmful until you realize that if there are std::shared_ptrs outside
the member function that already point to that Widget, it’s game, set, and match for
undefined behavior.

The std::shared_ptr API includes a facility for just this kind of situation. It has
probably the oddest of all names in the Standard C++ Library: std::
enable_shared_from_this. That’s a template for a base class you inherit from if
you want a class managed by std::shared_ptrs to be able to safely create a
std::shared_ptr from a this pointer. In our example, Widget would inherit from
std::enable_shared_from_this as follows:

class Widget: public std::enable_shared_from_this<Widget> {
public:
    …
    void process();
    …
};

std::enable_shared_from_this defines a member function that creates a
std::shared_ptr to the current object, but it does it without duplicating control
blocks. The member function is shared_from_this, and you use it in member func‐
tions whenever you want a std::shared_ptr that points to the same object as the
this pointer. Here’s a safe implementation of Widget::process:

void Widget::process()
{
    // as before, process the Widget
    …
    // add std::shared_ptr to current object to processedWidgets
    processedWidgets.emplace_back(shared_from_this());
}

Internally, shared_from_this looks up the control block for the current object, and
it creates a new std::shared_ptr that refers to that control block. The design relies
on the current object having an associated control block. For that to be the case, there
must be an existing std::shared_ptr (e.g., one outside the member function calling
shared_from_this) that points to the current object. If no such std::shared_ptr
exists (i.e., if the current object has no associated control block), behavior is unde‐
fined, although shared_from_this typically throws an exception.

*/