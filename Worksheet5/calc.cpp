/**
 * @file calc.cpp
 * @brief Main calculator application.
 */

#include <iostream>
#include "adder.h"

/**
 * @brief Main entry point of the program.
 *
 * Demonstrates use of the add function.
 *
 * @return 0 on success
 */
int main()
{
    int x = 6;
    int y = 7;

    std::cout << x << " + " << y << " = " << add(x, y) << std::endl;

    return 0;
}
