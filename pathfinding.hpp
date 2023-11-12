/*
 * File: pathfinding.hpp
 * Desc: A*-Pathfinding Algorithm Header-Only Implementation in C++
 */

#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include <vector>
#include <queue>
#include <string>
#include <stdexcept>
#include <functional>
#include <cmath>

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
        std::function<double(T from, T to)> movementCostFunction;
    public:
        // constructors

        // pathfinder-constructor for a grid of type int with preset (1:1) movement cost function.
        Pathfinder(const Grid<int>& grid) : 
        grid(grid), 
        movementCostFunction([&](int nodeFrom, int nodeTo) -> double { return double(nodeTo); }) {
            
        }
        // pathfinder-constructor for a grid of any type with user-definable movement cost function.
        Pathfinder(const Grid<T>& grid, const std::function<double(T from, T to)>& movementCostFunction) : 
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

        void setMovementCostFunction(std::function<double(T, T)>& movementCostFunction) {
            this->movementCostFunction = movementCostFunction;
        }

        // Getters
        const Grid<T>& getGrid() const {
            return grid;
        }
        const std::function<double(T, T)>& getMovementCostFunction() const {
            return movementCostFunction;
        }

    private:
        struct PathNode : public Node {
            double g, h, f;
            PathNode* parent; // parent node for path recreation

            PathNode(const Node& copy) {
                this->x = copy.x;
                this->y = copy.y;
                this->g = -1;
                this->h = -1;
                this->f = -1;
                this->parent = nullptr;
            }
        };

        struct CompareFunction {
            bool operator()(const PathNode* node1, const PathNode* node2) const {
                // Priority order: lower f-cost has higher priority
                if (node1->f == -1) return node2;
                if (node2->f == -1) return node1;

                return node1->f > node2->f;
            }
        };

        // euclidian heuristic cost
        double hCost(const Node& start, const Node& end) const {
            const double x = end.x - start.x, y = end.y - start.y;
            return std::sqrt(x * x + y * y);
        }

        std::vector<PathNode*> getNeighbors(PathNode* current, Grid<PathNode>& nodes, const Grid<double> move) {
            const Node moveSize = getGridSize(move);
            const Node nodesSize = getGridSize(nodes);

            std::vector<PathNode*> neighbors;

            for (int y = -moveSize.y / 2; y <= moveSize.y / 2; y++) {
                const int realY = current->y + y;
                if (realY < 0 || realY >= nodesSize.y) continue;
                for (int x = -moveSize.x / 2; x <= moveSize.x / 2; x++) {
                    const int realX = current->x + x;
                    if (realX < 0 || realX >= nodesSize.x) continue;

                    neighbors.push_back(&getGridValue(nodes, realX, realY));
                }
            }

            return neighbors;
        }

        // Function to reconstruct the path from start to goal
        void reconstructPath(PathNode* start, PathNode* goal, std::vector<Node*>& path) {
            PathNode* current = goal;

            while (current != nullptr) {
                path.push_back(current);
                current = current->parent;
            }

            path.reserve(sizeof(path));
        }

    public:
        // @param move must be a 3 by 3 grid 
        int find(Node startNode, Node endNode, std::vector<Node>& path, const Grid<double> move = { 
            { 1.4,   1, 1.4 },
            {   1,   0,   1 },
            { 1.4,   1, 1.4 }
        }) const {
            path.clear();
            Grid<PathNode> pathnodes;
            std::priority_queue<PathNode*, std::vector<PathNode*>, CompareFunction> list;
            const Node size = getGridSize(this->grid);
            for (int y = 0; y < size.y; y++) {
                std::vector<PathNode> pathnode_list;
                for (int x = 0; x < size.x; x++) {
                    pathnode_list.emplace_back(Node(x, y));
                    list.push(pathnode_list.back());
                }
                pathnodes.emplace_back(pathnode_list);
            }

            PathNode& start = getGridValue(pathnodes, startNode), end = getGridValue(pathnodes, endNode);
            start.g = 0;
            start.h = hCost(startNode, endNode);
            start.f = start.g + start.h;
            start.parent = nullptr;
            list.push(&start);

            while (!list.empty()) {
                PathNode* current = list.top();
                list.pop();

                // check if end is reached
                if (current == &end) {
                    reconstructPath(start, end, path);
                    return 0;
                }

                // Explore neighbors
                for (PathNode* neighbor : getNeighbors(current, pathnodes, move)) {
                    double tentativeG = 
                        current->g + cost(current, neighbor) * 
                        getGridValue(move, neighbor->x - current->x + 1, neighbor->y - current->y + 1);

                    if (tentativeG < neighbor->g || neighbor->g == -1) {
                        neighbor->parent = current;
                        neighbor->g = tentativeG;
                        neighbor->h = calculateHeuristic(*neighbor, endNode);
                        neighbor->f = neighbor->g + neighbor->h;
                    }
                }
            }

            // no valid path found
            return 1;
        }
    };
} // namespace pathfinding

#endif