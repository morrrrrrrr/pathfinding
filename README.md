# pathfinding.hpp

A*-Pathfinding Algorithm Header-Only Implementation in C++

## Description

`pathfinding.hpp` is a Header-Only C++ library implementing the A* pathfinding algorithm. It provides a simple and efficient way to find the shortest path between two points on a grid.

![Alt-Text](explanation.gif)

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

C++ 17 or later

## Documentation

These are the Types introduced in `namespace pathfinding`

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

#### Grid Creation Use Example (from tests/test.cpp):
```cpp
const pathfinding::Grid<int> grid ({
    { 0, -1, 0, 0, 0 },
    { 0, -1, 0, -1, 0 },
    { 0, -1, 0, -1, 0 },
    { 0, -1, 0, -1, 0 },
    { 0, 0, -1, 0, 0 }
});
```

#### Related Functions:

 - [Constructors](#1-constructors)
 - [Access](#2-access-functions)
 - [Getter](#3-getter-functions)
 - [Iterator](#4-iterator-functions)

##### 1. **Constructors**
This initialized the grid with the data specified (see [Grid-Creation](#grid-creation-use-example-from-teststestcpp))
```cpp
Grid(std::vector<std::vector<T>> data)
```
This initialized the grid with the default value for T for size X,Y
```cpp
Grid(const Node& size)
Grid(const int sizeX, const int sizeY)
```

##### 2. **Access-Functions**
 - Mutable
```cpp
T& at(const int x, const int y);
T& at(const Node& node);
T& operator[](const Node& node);
```
 - Immutable
```cpp
const T& at(const int x, const int y) const;
const T& at(const Node& node) const;
const T& operator[](const Node& node) const;
```

##### 3. **Getter-Functions**
Returns the data Size in a node: (as in a vector2int-like structure)
```cpp
const Node getSize() const;
```
returns `true` if the indexes are in bounds of the data
```cpp
bool inBounds(const int x, const int y) const;
bool inBounds(const Node& node) const;
```

##### 4. **Iterator-Functions**
These return a (const-)`iterator` to the y row of the grid data (for iterator based loops (see [Iterator-Loops](#functions-use-example-from-teststestcpp)))
```cpp
 // returns the begin of the y-Rows
typename std::vector<std::vector<T>>::iterator begin();
// returns the end of the y-Rows
typename std::vector<std::vector<T>>::iterator end();
// returns the begin of the y-Rows
typename std::vector<std::vector<T>>::const_iterator begin() const;
// returns the end of the y-Rows
typename std::vector<std::vector<T>>::const_iterator end() const;
```

#### Functions Use Example (from tests/test.cpp):

1. **Index-based Loop**
```cpp
const pf::Node size = grid.getSize();
for (int y = 0; y < size.y; y++) {
    for (int x = 0; x < size.x; x++) { 
        std::cout << std::to_string(grid.at(x, y));
    }
    std::cout << "\n";
}
```
2. **Iterator-based Loop**
```cpp
for (const auto& yRow : grid) {
    for (const auto& element : yRow) {
        std::cout << element;
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

4. **Customizability:**
   - The Pathfinding library can be included with the following flag `PATHFINDING_CALLBACKS` to enable callback functions in the pathfinding process for easy visualization

#### Movement Cost Function

The movement cost function (`MCF`) defines the cost to move from `T from` to `T to`.
The `MCF` returns a `double` and takes a `T from` and `T to`

if the `MCF` returns a number lower than 0, the field is treated as `intraversable`

This is the automatic preset `MCF` for an `int`-Pathfinder.
```cpp
[&](int nodeFrom, int nodeTo) -> double { 
    if (nodeTo < 0)
        return -1;
    if (nodeTo >= 0)
        return nodeTo + 1; 
}
```

#### Callbacks

 - On Node Popped:
```cpp
// this function gets called when the algorithm poppes (calculates) a node
std::function<void(const Node& node)> onPoppedNodeCallback;
```
 - On Path Added: At the end of the Algorithm when the path gets constructed, this function gets called for each node that is added to the final path
```cpp
// this function gets called when a new node is added to the output path
std::function<void(const Node& node)> onPathAddedCallback;
```

To include these Callbacks define `PATHFINDING_CALLBACKS` before including the library:
```cpp
#define PATHFINDING_CALLBACKS // include the callback functions
#include <pathfinding.hpp>
```

#### Pathfinder Use Example (from tests/test.cpp):

```cpp
namespace pf = pathfinding;
// Create Path vector
std::vector<pf::Node> path;
pathfinder.find(pf::Node(4, 4), pf::Node(0, 0), path);
```

#### Pathfinder Functions

 - [Constructors](#1-constructors-1)
 - [Setter-Functions](#2-setter-functions)
 - [Getter-Functions](#3-getter-functions-1)
 - [Pathfind-Functions](#4-pathfind-function)

##### 1. **Constructors**

A constructor for an `integer`-based Grid/Pathfinder. 
The `MCF` is pre-set to output the grid value
```cpp
Pathfinder(const Grid<int>& grid)
```
A constructor for a `T`-based Grid with custom `MCF`
```cpp
Pathfinder(const Grid<T>& grid, const std::function<double(T from, T to)>& movementCostFunction)
```
A constructor for a `T`-based Grid with custom `MCF` and custom callback functions (Only enabled with `CALLBACKS`-Flag)
```cpp
Pathfinder(const Grid<T>& grid, const std::function<double(T from, T to)>& movementCostFunction, 
    const std::function<void(const Node& node)>& onPoppedNodeCallback, 
    const std::function<void(const Node& node)>& onPathAddedCallback)
```

##### 2. **Setter-Functions**

```cpp
void setGrid(const Grid<T>& grid)
void setMovementCostFunction(std::function<double(T, T)>& movementCostFunction)
```
Only with `CALLBACKS`-Flag:
```cpp
void setPoppedNodeCallback(std::function<void(const Node& node)> onPoppedNodeCallback)
void setPathAddedCallback(std::function<void(const Node& node)> onPathAddedCallback)
```

##### 3. **Getter-Functions**

```cpp
const Grid<T>& getGrid() const
const std::function<double(T, T)>& getMovementCostFunction() const
```
Only with `CALLBACKS`-Flag:
```cpp
const std::function<void(const Node& node)>& getPoppedNodeCallback()
const std::function<void(const Node& node)>& getPathAddedCallback()
```

##### 4. **Pathfind-Functions**

Main Pathfind function:

 - `startNode`: The starting node for the pathfinding algorithm.
 - `endNode`:   The target or goal node that the algorithm is trying to reach.
 - `path`:      A reference to a vector of nodes. This vector will be populated with the nodes representing the computed path from the start node to the end node.
 - `move`:      A grid of movement costs. This grid likely represents the cost of moving from one node to another and is used by the pathfinding algorithm to determine the optimal path.
 - `return`:    The function returns an integer; 0 => a valid path was found, 1 => no valid path was found

Move has a default value:
```cpp
const Grid<double>& move = Grid<double>({ 
    { 1.4,   1, 1.4 },
    {   1,  -1,   1 },
    { 1.4,   1, 1.4 }
})
```

```cpp
int find(Node startNode, Node endNode, std::vector<Node>& path, const Grid<double> move) const
```

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.