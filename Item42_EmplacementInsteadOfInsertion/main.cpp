
/**
• In principle, emplacement functions should sometimes be more efficient than
their insertion counterparts, and they should never be less efficient.
• In practice, they’re most likely to be faster when (1) the value being added is
constructed into the container, not assigned; (2) the argument type(s) passed
differ from the type held by the container; and (3) the container won’t reject
the value being added due to it being a duplicate.
• Emplacement functions may perform type conversions that would be rejected
by insertion functions.
 */

#include <vector>

#include "Chronometer.h"


int main() {

    utils::Chronometer chronometer;

    std::vector<std::string> vecE;
    std::vector<std::string> vecI;

    // Warm=up
    for(int i = 0; i < 1000; ++i) {
        vecI.emplace_back("xx");
    }
    vecI.clear();
    
    for(int i = 0; i < 1000; ++i) {
        vecE.push_back("xx");
    }
    vecE.clear();

    // Emplacement
    chronometer.Start();
    for(int i = 0; i < 1000; ++i) {
        vecI.emplace_back("xyxyx");
    }
    chronometer.Stop();
    chronometer.Report("Emplacement of rvalue");

    // Insertion
    chronometer.Start();
    for(int i = 0; i < 1000; ++i) {
        vecE.push_back("xyxyx");
    }
    chronometer.Stop();
    chronometer.Report("Insertion of rvalue");
    vecI.clear();
    vecE.clear();

    std::string queenOfDisco("Donna Summer");
    // Emplacement
    chronometer.Start();
    for(int i = 0; i < 1000; ++i) {
        vecI.emplace_back(queenOfDisco);
    }
    chronometer.Stop();
    chronometer.Report("Emplacement of lvalue");

    // Insertion
    chronometer.Start();
    for(int i = 0; i < 1000; ++i) {
        vecE.push_back(queenOfDisco);
    }
    chronometer.Stop();
    chronometer.Report("Insertion of lvalue");

    // Emplacement to occupied index (avoid)
    chronometer.Start();
    for(int i = 0; i < 1000; ++i) {
        vecI.emplace(vecI.begin(), "xyxyx");
    }
    chronometer.Stop();
    chronometer.Report("Emplacement of rvalue to occupied index");


    

    return 0;
}