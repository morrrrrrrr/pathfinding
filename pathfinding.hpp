/*
 * File: pathfinding.hpp
 * Desc: A*-Pathfinding Algorithm Header-Only Implementation in C++
 */

#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include <vector>
#include <string>
#include <stdexcept>
#include <functional>

namespace pathfinding {
    // Structure representing a 2D point.
    struct Node {
        int x, y;
        
        // Constructors
        Node() : x(0), y(0) { }
        Node(int x, int y) : x(x), y(y) { }

        ~Node() { }
        // copy constructor
        Node(const Node& other) :
            x(other.x), 
            y(other.y) {
            
        }
        Node(Node&& other) noexcept : 
            x(std::move(other.x)), 
            y(std::move(other.y)) {
            
        }
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
        // any movement cost under 0 means the field is untraversable
        std::function<double(T)> movementCostFunction;
    public:
        // constructors

        // pathfinder-constructor for a grid of type int with preset (1:1) movement cost function.
        Pathfinder(const Grid<int>& grid) : 
        grid(grid), 
        movementCostFunction([&](int input) -> double { return double(input); }) {
            
        }
        // pathfinder-constructor for a grid of any type with user-definable movement cost function.
        Pathfinder(const Grid<T>& grid, const std::function<double(T)>& movementCostFunction) : 
            grid(grid), 
            movementCostFunction(movementCostFunction) { 
            
        }

        // destructor
        ~Pathfinder() {

        }

        // copy constructor
        Pathfinder(const Pathfinder& other) : 
            grid(other.grid), 
            movementCostFunction(other.movementCostFunction) {

        }

        // move constructor
        Pathfinder(Pathfinder&& other) noexcept : 
            grid(std::move(other.grid)), 
            movementCostFunction(std::move(other.movementCostFunction)) {

        }

        // Setters
        void setGrid(const Grid<T>& grid) {
            this->grid = grid;
        }

        void setMovementCostFunction(std::function<double(T)>& movementCostFunction) {
            this->movementCostFunction = movementCostFunction;
        }

        // Getters
        const Grid<T>& getGrid() {
            return grid;
        }
        const std::function<double(T)>& getMovementCostFunction() {
            return movementCostFunction;
        }
    };
} // namespace pathfinding

#endif