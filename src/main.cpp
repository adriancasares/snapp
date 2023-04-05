#include <iostream>

#include "token.h"

int main() {
    auto symbol = Snapp::findSymbol("!=");
    std::cout << symbol.value_or(Snapp::Symbol::Unknown) << std::endl;
}
