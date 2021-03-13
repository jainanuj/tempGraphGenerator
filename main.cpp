//
//  main.cpp
//  temporalGraphGenerator
//
//  Created by Anuj Jain on 1/10/21.
//

//This program takes an input a file with edges in a static graph (u, v).
//It creates two output files:
//    1. A temporal graph with random number of time intervals on each edge, random duration of each interval and random travel times in each interval. All these intervals are non-overlapping
//    2. Another model of the same graph with sequence of edges of the form (u, v, t, lambda), sorted by the time instance of each of hte edges.

#include <iostream>
#include "Graph.hpp"

int main(int argc, const char * argv[]) {
    Graph g(argv[1]);

    std::cout << "Hello, World!\n";
    return 0;
}
