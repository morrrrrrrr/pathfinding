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
#include <algorithm>

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
        
        Node operator=(const Node& other) {
            this->x = other.x;
            this->y = other.y;
        }

        bool operator==(const Node& other) const {
            return this->x == other.x && this->y == other.y;
        }
    };

    // Template for a 2D grid.
    template<typename T>
    class Grid {
    private:
        std::vector<std::vector<T>> data;
    public:
        Grid() {

        }
        Grid(std::vector<std::vector<T>> data) : data(data) {
            
        }

        ~Grid() {

        }

        Grid(const Grid& copy) : data(copy.data) {

        }
        Grid(Grid&& move) : data(std::move(move.data)) {
            
        }
        Grid operator=(const Grid& copy) {
            this->data = copy.data;
        }
        Grid operator=(Grid&& move) {
            this->data = std::move(move.data);
        }

        // returns the begin of the y-Rows
        typename std::vector<std::vector<T>>::iterator begin() {
            return data.begin();
        }
        // returns the end of the y-Rows
        typename std::vector<std::vector<T>>::iterator end() {
            return data.end();
        }
        // returns the begin of the y-Rows
        typename std::vector<std::vector<T>>::const_iterator begin() const {
            return data.begin();
        }
        // returns the end of the y-Rows
        typename std::vector<std::vector<T>>::const_iterator end() const {
            return data.end();
        }

        const Node getSize() const {
            // check if grid is completely empty before accessing row 0
            if (data.size() == 0) return Node(0, 0);
            // there is atleast one row in y, so data[0] is completely safe
            return Node(data[0].size(), data.size());
        }

        bool inBounds(const int x, const int y) const {
            const Node size = this->getSize();
            return (x >= 0 && x < size.x || y >= 0 || y < size.y);
        }
        bool inBounds(const Node& node) const {
            return inBounds(node.x, node.y);
        }

        // mutable
        T& at(const int x, const int y) {
            try { // Handle out-of-range access gracefully.
                return data.at(y).at(x);
            }
            catch (std::out_of_range& err) {
                throw std::out_of_range("Grid access error: " + std::string(err.what()));
            }
        }
        T& at(const Node& node) {
            try { // Handle out-of-range access gracefully.
                return data.at(node.y).at(node.x);
            }
            catch (std::out_of_range& err) {
                throw std::out_of_range("Grid access error: " + std::string(err.what()));
            }
        }
        T& operator[](const Node& node) {
            return this->at(node);
        }

        // immutable
        const T& at(const int x, const int y) const {
            try { // Handle out-of-range access gracefully.
                return data.at(y).at(x);
            }
            catch (std::out_of_range& err) {
                throw std::out_of_range("Grid access error: " + std::string(err.what()));
            }
        }
        const T& at(const Node& node) const {
            try { // Handle out-of-range access gracefully.
                return data.at(node.y).at(node.x);
            }
            catch (std::out_of_range& err) {
                throw std::out_of_range("Grid access error: " + std::string(err.what()));
            }
        }
        const T& operator[](const Node& node) const {
            return this->at(node);
        }
    };

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
        movementCostFunction([&](int nodeFrom, int nodeTo) -> double { 
            if (nodeTo < 0)
                return -1;
            if (nodeTo >= 0)
                return nodeTo + 1; 
        }) {
            
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

        class CompareFunction {
        public:
            bool operator()(const PathNode* node1, const PathNode* node2) const {
                // Priority order: lower f-cost has higher priority
                if (node1->f == -1) return node2;
                if (node2->f == -1) return node1;

                return node1->f < node2->f;
            }
        };

        // euclidian heuristic cost
        double hCost(const Node& start, const Node& end) const {
            const double x = end.x - start.x, y = end.y - start.y;
            return std::sqrt(x * x + y * y);
        }

        std::vector<PathNode*> getNeighbors(PathNode* current, Grid<PathNode>& nodes, const Grid<double> move) const {
            const Node moveSize = move.getSize();
            const Node nodesSize = nodes.getSize();

            std::vector<PathNode*> neighbors;

            for (int y = 0; y < moveSize.y; y++) {
                const int realY = current->y + y - moveSize.y / 2;
                if (realY < 0 || realY >= nodesSize.y) continue;
                for (int x = 0; x < moveSize.x; x++) {
                    const int realX = current->x + x - moveSize.x / 2;
                    if (realX < 0 || realX >= nodesSize.x) continue;
                    // move has to be flipped, because the algorithm works backwards
                    if (move.at(moveSize.x - x - 1, moveSize.y - y - 1) <= 0) continue;

                    neighbors.push_back(&nodes.at(realX, realY));
                }
            }

            return neighbors;
        }

        // Function to reconstruct the path from start to goal
        void reconstructPath(PathNode* end, std::vector<Node>& path) const {
            PathNode* current = end;
            while (current != nullptr) {
                path.push_back(*current);
                current = current->parent;
            }
        }

        PathNode* getAndRemoveTop(std::vector<PathNode*>& pathlist) const {
            std::sort(pathlist.begin(), pathlist.end(), [&](const PathNode* node1, const PathNode* node2) -> bool {
                if (node1->f == -1) return false;
                if (node2->f == -1) return true;

                return node1->f < node2->f;
            });
            PathNode* top = pathlist.at(0);
            pathlist.erase(pathlist.begin());
            return top;
        }

    public:
        // Main pathfinding function
        // @param startNode: The node where the path starts
        // @param endNode: The node where the path ends
        // @param move: A 3x3 grid defining movement costs
        // @param path: A vector to store the computed path
        // @return 0 if a path was found, 1 if no valid path was found
        int find(Node startNode, Node endNode, std::vector<Node>& path, const Grid<double>& move) const {
            path.clear();
            std::vector<std::vector<PathNode>> nodes;
            const Node size = this->grid.getSize();
            for (int y = 0; y < size.y; y++) {
                std::vector<PathNode> pathnode_list;
                for (int x = 0; x < size.x; x++) {
                    pathnode_list.push_back(PathNode(Node(x, y)));
                }
                nodes.push_back(pathnode_list);
            }
            Grid<PathNode> pathnodes(nodes);
            std::vector<PathNode*> nodeList;
            for (int x = 0; x < size.x; x++) {
                for (int y = 0; y < size.y; y++) {
                    nodeList.push_back(&pathnodes.at(x, y));
                }
            }
            
            // start from end and end on start
            // the output path normally is reverse. Instead of reversing the output path vector
            // run the algorithm in reverse and have the path output as normal
            // -> switch start and end node
            PathNode* start = &pathnodes[endNode], *end = &pathnodes[startNode];

            start->g = 0;
            start->h = hCost(*start, *end);
            start->f = start->g + start->h;
            start->parent = nullptr;

            while (nodeList.size()) {
                PathNode* current = getAndRemoveTop(nodeList);

                // unpopped nodes have g < 0
                // if the top node is unpopped, no correct solution exists
                if (current->g < 0) {
                    return 1;
                }

                // check if end is reached
                if (Node(*current) == Node(*end)) {
                    reconstructPath(current, path);
                    return 0;
                }

                // Explore neighbors
                for (PathNode* neighbor : getNeighbors(current, pathnodes, move)) {
                    // pop node
                    // move has to be flipped, because the algorithm works backwards
                    double rawMovementCost = movementCostFunction(grid[static_cast<Node>(*current)], grid[static_cast<Node>(*neighbor)]) * 
                        move.at(move.getSize().x - (neighbor->x - current->x + 2), move.getSize().y - (neighbor->y - current->y + 2));
                        // previos (unflipped move):
                        //move.at(neighbor->x - current->x + 1, neighbor->y - current->y + 1);
                    double tentativeG = current->g + rawMovementCost;
                    
                    // g cost < 0 means intraversable node
                    if (rawMovementCost < 0)
                        continue;

                    // neighbor->g < 0 would mean that the neighbor is unset
                    if (tentativeG < neighbor->g || neighbor->g < 0) {
                        neighbor->parent = current;
                        neighbor->g = tentativeG;
                        neighbor->h = hCost(*neighbor, *end);
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