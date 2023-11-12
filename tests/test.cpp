
// include pathfinding.hpp from folder below (this may change in a use-case)
#include "../pathfinding.hpp"

#include <vector>
#include <iostream>
#include <string>

void printGrid(const pathfinding::Grid<int> grid) {
    for (const auto& yRow : grid) {
        for (const auto& element : yRow) {
            std::cout << element;
        }
        std::cout << "\n";
    }
}

void testGridFunctions(const pathfinding::Grid<int>& grid) {
    namespace pf = pathfinding;

    std::cout << "Test of the Get-Functions:\n";

    const pf::Node size = pf::getGridSize(grid);
    std::cout << "\tPrint grid size:\n";
    // in node.x is the grid X size, int node.y the grid Y size
    std::cout << "Size X: " << size.x << ", Size Y: " << size.y << "\n";
    // Size X: 5, Size Y: 5

    std::cout << "\tPrint the whole Grid:\n";
    // loop through the grid

    for (int y = 0; y < size.y; y++) {
        for (int x = 0; x < size.x; x++) { 
            std::cout << std::to_string(pf::getGridValue(grid, x, y));
        }
        std::cout << "\n";
    }
    /*
        00000
        00000
        01110
        00010
        00000
    */

    // this throws out of range exception:
    // const int i = pf::getGridValue(grid, -1, -1);

    // copying the grid:
    pf::Grid<int> newGrid = pf::Grid<int>(grid);
    // print the new Grid (this time through iterators)
    std::cout << "\n\tPrint copied Grid:\n";
    printGrid(newGrid);
    /*
        00000
        00000
        01110
        00010
        00000
    */

    // changing a value then printing again:

    // change the top left to 9:
    pf::getGridValue(newGrid, 0, 0) = 9;
    // explanation: newGrid is mutable, so getGridValue returns a mutable reference of the position in the grid

    std::cout << "\n\tPrint modified Grid:\n";
    printGrid(newGrid);
    /*
        90000
        00000
        01110
        00010
        00000
    */
}

void testPathfinderFunctions(const pathfinding::Pathfinder<int> pathfinder) {
    
}

int main(int argc, char** argv) {
    const pathfinding::Grid<int> grid {
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 1, 1, 1, 0 },
        { 0, 0, 0, 1, 0 },
        { 0, 0, 0, 0, 0 }
    };

    testGridFunctions(grid);

    pathfinding::Pathfinder<int> pathfinder(grid);

    testPathfinderFunctions(pathfinder);

    return 0;
}
