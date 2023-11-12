
// include pathfinding.hpp from folder below (this may change in a use-case)
#include "../pathfinding.hpp"

#include <vector>
#include <iostream>
#include <string>

void testGetFunctions(const pathfinding::Grid<int>& grid) {
    namespace pf = pathfinding;

    std::cout << "Test of the Get-Functions:\n";
    std::cout << "\tPrint the whole Grid:\n";
    // loop through the grid
    for (int y = 0; y < pf::getGridSize(grid).y; y++) {
        for (int x = 0; x < pf::getGridSize(grid).x; x++) { 
            std::cout << std::to_string(pf::getGridValue(grid, x, y));
        }
        std::cout << "\n";
    }

    std::cout << "\tPrint grid size:\n";
    // in node.x is the grid X size, int node.y the grid Y size
    const pf::Node size = pf::getGridSize(grid);
    std::cout << "Size X: " << size.x << ", Size Y: " << size.y << "\n";

    // throws exception correctly
    // const int i = pf::getGridValue(grid, -1, -1);
}

int main(int argc, char** argv) {
    const pathfinding::Grid<int> grid {
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 1, 1, 1, 0 },
        { 0, 0, 0, 1, 0 },
        { 0, 0, 0, 0, 0 }
    };

    testGetFunctions(grid);

    return 0;
}
