/*
 * File: pathfinding.hpp
 * Desc: 
 */

#define PATHFINDING_HPP
#ifndef PATHFINDING_HPP

#include <vector>

namespace pathfinding {
    struct Node {
        int x, y;
        
        // Constructors
        Node() : x(0), y(0) { }
        Node(int x, int y) : x(x), y(y) { }
    };

    using Grid = std::vector<std::vector<int>>;

    static int operator[](const Grid& grid, const Node& node) {
        return grid[node.x][node.y];
    }
} // namespace pathfinding

#endif