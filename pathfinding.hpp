/*
 * File: pathfinding.hpp
 * Desc: 
 */

#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include <vector>
#include <string>
#include <stdexcept>

namespace pathfinding {
    // Structure representing a 2D point.
    struct Node {
        int x, y;
        
        // Constructors
        inline Node() : x(0), y(0) { }
        inline Node(int x, int y) : x(x), y(y) { }
    };

    // Template for a 2D grid.
    template<typename T>
    using Grid = std::vector<std::vector<T>>;

    // Access a mutable value in the grid based on a Node.
    template<typename T>
    inline T& getGridValue(Grid<T>& grid, const Node& node) {
        try { // Handle out-of-range access gracefully.
            return grid.at(node.y).at(node.x); 
        }
        catch (std::out_of_range& err) {
            throw std::out_of_range("Grid access error: " + std::string(err.what()));
        }
    }
    // Access a mutable value in the grid based on x and y coordinates.
    template<typename T>
    inline T& getGridValue(Grid<T>& grid, const int& x, const int& y) {
        try { // Handle out-of-range access gracefully.
            return grid.at(y).at(x); 
        }
        catch (std::out_of_range& err) {
            throw std::out_of_range("Grid access error: " + std::string(err.what()));
        }
    }

    // Access an immutable value in the grid based on a Node.
    template<typename T>
    inline const T& getGridValue(const Grid<T>& grid, const Node& node) {
        try { // Handle out-of-range access gracefully.
            return grid.at(node.y).at(node.x); 
        }
        catch (std::out_of_range& err) {
            throw std::out_of_range("Grid access error: " + std::string(err.what()));
        }
    }
    // Access an immutable value in the grid based on x and y coordinates.
    template<typename T>
    inline const T& getGridValue(const Grid<T>& grid, const int& x, const int& y) {
        try { // Handle out-of-range access gracefully.
            return grid.at(y).at(x); 
        }
        catch (std::out_of_range& err) {
            throw std::out_of_range("Grid access error: " + std::string(err.what()));
        }
    }

    // Get the size of the grid. returns the size as a Node-struct
    // (Node.x => grid size x) 
    // (Node.y => grid size y)
    template<typename T>
    inline const Node getGridSize(const Grid<T>& grid) {
        return Node(grid[0].size(), grid.size());
    }

    // A template class for the a*-pathfinding algorithms.
    template<typename T>
    class Pathfinder {
        Grid<T> grid;
    public:
        Pathfinder(const Grid<T>& grid) : grid(grid) { };
    };
} // namespace pathfinding

#endif