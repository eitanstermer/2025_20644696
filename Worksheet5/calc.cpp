#include <iostream>
#include "adder.h"

/**
 * @brief Main function.
 * 
 * Demonstrates the use of the add function.
 */
int main() {
    int result = add(5, 3);
    std::cout << "5 + 3 = " << result << std::endl;
    return 0;
}
