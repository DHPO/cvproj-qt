#include <string>
#include <iostream>
#include "./expect.h"
#define ABORT true

void expect(bool expr, std::string errMsg)
{
    if (!expr) {
        if (ABORT) {
            std::cerr << "[Error] " << errMsg << std::endl;
            throw(errMsg);
        }
        else
            std::cerr << "[Error] " << errMsg << std::endl;
    }
}
