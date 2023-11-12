# pathfinding.hpp

A*-Pathfinding Algorithm Header-Only Implementation in C++

## Description

`pathfinding.hpp` is a Header-Only C++ library implementing the A* pathfinding algorithm. It provides a simple and efficient way to find the shortest path between two points on a grid.

## Table of Contents

- [Installation](#installation)
- [Requirements](#requirements)
- [Documentation](#documentation)
- [License](#license)

## Installation

To use `pathfinding.hpp` in your C++ project, follow these steps:

1. Copy the `pathfinding.hpp` header file into your project's include directory
2. Include the header file in your project

```cpp
#include "include/pathfinding.hpp"
```

3. Done

## Requirements

C++ 14 or later

## Documentation

All of the introduced types are under `namespace pathfinding`
Introduced Types:
- [Node](#node)
- [Grid](#grid)
- [Pathfinder](#pathfinder)

### Node
Definintion:
```cpp
struct Node {
    int x, y;
}
```
Constructors:
Initializes x and y as 0, 0
```cpp
Node() : x(0), y(0) { 

}
```
Initializes x and y as defined values
```cpp
Node(int x, int y) : x(x), y(y) { 

}
```

### Grid

Grid is a matrix-type-object of any Type

#### Definition:
```cpp
template<typename T>
using Grid = std::vector<std::vector<T>>;
```
#### Grid Creation Use Example (from tests/test.cpp):
```cpp
const pathfinding::Grid<int> grid {
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 0 },
    { 0, 0, 0, 1, 0 },
    { 0, 0, 0, 0, 0 }
};
```

#### Related Functions:
```cpp
// get immutable Element from immutable grid
const T& getGridValue(const Grid<T>& grid, const Node& node); // through const node
const T& getGridValue(const Grid<T>& grid, const int x, const int y); // through x and y

// get mutable Element from mutable grid
T& getGridValue(Grid<T>& grid, const Node& node); // through const node
T& getGridValue(Grid<T>& grid, const int x, const int y); // through x and y

// get Grid Size as Node where node.x is the grid size x and node.y is the grid size y
const Node getGridSize(const Grid<T>& grid);
```

#### Functions Use Example (from tests/test.cpp):
Application case: Printing a whole grid through manual iteration:
```cpp
const pf::Node size = pf::getGridSize(grid);
for (int y = 0; y < size.y; y++) {
    for (int x = 0; x < size.x; x++) { 
        std::cout << std::to_string(pf::getGridValue(grid, x, y));
    }
    std::cout << "\n";
}
```

### Pathfinder

The `Pathfinder` class encapsulates essential functionality for pathfinding algorithms. It provides a versatile framework for working with 2D grids of any data type. Key features include:

1. **Grid Storage:**
   - The class maintains a grid of any data type, allowing flexibility in representing various types of environments or mazes.

2. **Customizable Movement Cost:**
   - Users can define a function to determine the movement cost associated with each value present in the grid. This flexibility allows adapting the pathfinding algorithm to different scenarios.

3. **Pathfinding Logic:**
   - The class incorporates the core logic for pathfinding algorithms, providing a foundation for finding optimal paths within the specified grid.

#### Definition

```cpp
template<typename T>
class Pathfinder {
    Grid<T> grid;
    // any movement cost under 0 means the field is untraversable
    std::function<double(T)> movementCostFunction;
public:
    // pathfinder-constructor for a grid of type int with preset (1:1) movement cost function.
    Pathfinder(const Grid<int>& grid);
    // pathfinder-constructor for a grid of any type with user-definable movement cost function.
    Pathfinder(const Grid<T>& grid, const std::function<double(T)>& movementCostFunction);

    // destructor
    ~Pathfinder();

    // copy constructor
    Pathfinder(const Pathfinder& other);

    // move constructor
    Pathfinder(Pathfinder&& other) noexcept;

    // Setters
    void setGrid(const Grid<T>& grid);
    void setMovementCostFunction(std::function<double(T)>& movementCostFunction);

    // Getters
    const Grid<T>& getGrid();
    const std::function<double(T)>& getMovementCostFunction();
}
```

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.