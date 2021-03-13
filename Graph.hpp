//
//  Graph.hpp
//  temporalGraphGenerator
//
//  Created by Anuj Jain on 1/10/21.
//

#ifndef Graph_hpp
#define Graph_hpp

#include <stdio.h>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <set>
#include <queue>
#include <iostream>
using namespace std;

struct Interval
{
    int intvlStart;
    int intvlEnd;
    int traveTime;
};

class Graph
{
public:
    Graph() {}
    Graph(const char* filePath); // input file
    void xuanToWuModel(const char* filePath); // input file
    void finalizeOutput(int numVertices, int numEdges, string opFile, string outputFinal);
    vector<tuple<int, int, int>> adjustSlowIntervals(std::vector<tuple<int, int, int> > &intervalVector);
};

class compareTuple {
public:
    bool operator () (std::tuple<int, int, int, int> tup1, std::tuple<int, int, int, int> tup2)
    {
        if (std::get<2>(tup1) != std::get<2>(tup2))
            return (std::get<2>(tup1) < std::get<2>(tup2));
        else if (std::get<0>(tup1) != std::get<0>(tup2))
            return (std::get<0>(tup1) < std::get<0>(tup2));
        else if (std::get<1>(tup1) != std::get<1>(tup2))
            return (std::get<1>(tup1) < std::get<1>(tup2));
        else
            return (std::get<3>(tup1) < std::get<3>(tup2));
    }
};





#endif /* Graph_hpp */
