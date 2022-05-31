#include <iostream>

#include "enumtest.h"
#include "status.h"

void continueProcessing(Status s, Color c) {
    if(s == Status::good) {
        std::cout << "good" << std::endl;
    }

    if(c == brown) {
        std::cout << "brown" << std::endl;
    }
}