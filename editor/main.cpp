#include <iostream>
#include "RizzEngine.h"



int main() {
    RIZZ::Logger::Init();

    RZ_CORE_WARN("Initilized Log!");
    RZ_INFO("Hello!");



    std::cin.get();
    return 0;
}