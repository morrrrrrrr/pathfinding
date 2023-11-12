// This is a test of the pathfinding.hpp library
// This uses lodepng from (https://github.com/lvandeve/lodepng) to load and save png images and visualize the path finding algorithm

// include the lodepng library
#include "lodepng.h" // excluded from git

#define PATHFINDING_CALLBACKS // include the callback functions
#include "../pathfinding.hpp"

#include <vector>
#include <iostream>
#include <string>

// print grid function from test.cpp (for debug reading in grid, etc)
void printGrid(const pathfinding::Grid<int> grid) {
    for (const auto& yRow : grid) {
        for (const auto& element : yRow) {
            std::cout << element;
        }
        std::cout << "\n";
    }
}

std::vector<unsigned char> image; //the raw pixels
unsigned width, height; 

size_t counter = 0;

void onNodePopped(const pathfinding::Node& node) {
    image[node.y * width * 4 + node.x * 4 + 0] = 0; 
    image[node.y * width * 4 + node.x * 4 + 1] = image[node.y * width * 4 + node.x * 4 + 1]; 
    image[node.y * width * 4 + node.x * 4 + 2] = 0; 
}

void onPathAdded(const pathfinding::Node& node) {
    //std::cout << node.x << ", " << node.y << "\n";
    image[node.y * width * 4 + node.x * 4 + 0] = 255;
    image[node.y * width * 4 + node.x * 4 + 1] = 0;
    image[node.y * width * 4 + node.x * 4 + 2] = 0;
}

int main(int argc, char **argv) {
    namespace pf = pathfinding;

    //decode
    unsigned error = lodepng::decode(image, width, height, "maze.png");

    //if there's an error, display it
    if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    pathfinding::Grid<int> grid(width, height);

    std::size_t counter = 0;
    for (auto& yRow : grid) {
        for (auto& element : yRow) {
            // check rgb NOT a
            // check if pixel is black: then set to -1, else set to 0
            element = ((image.at(counter + 0) + image.at(counter + 1) + image.at(counter + 2)) == 0) ? -1 : 0;
            counter += 4;
        }
    }

    //printGrid(grid);

    pf::Pathfinder<int> pathfinder(grid, [&](int from, int to) {
        if (to < 0)
            return -1;
        if (to >= 0)
            return to + 1;
    }, onNodePopped, onPathAdded);

    std::vector<pf::Node> path;
    
    pathfinder.find(pf::Node(0, 0), pf::Node(width - 1, height - 1), path, pf::Grid<double>({ 
        { -1,  1, -1 },
        {  1, -1,  1 },
        { -1,  1, -1 }
    }));
    
    /*
    pathfinder.find(pf::Node(0, 0), pf::Node(width - 1, height - 1), path);
    */
    std::cout << "Saving image!\n";
    error = lodepng::encode("maze_solved.png", image, width, height);

    if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}
