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
#include <tuple>
#include <iostream>
using namespace std;
#define MAX_RANGE 500
#define STD_DEV 2
#define AVG_NUM_INTVLS 2
#define AVG_INTVL_DURATION 5
#define AVG_TRVL_TIME 3


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
    Graph(const char* filePath, int avgNumIntvls=AVG_NUM_INTVLS, int avgInvlDuration=AVG_INTVL_DURATION, int avgTrvlTime=AVG_TRVL_TIME, int contactSeq=0);
    void xuanToWuModel(const char* filePath, int contactSeq); // input file
    void finalizeOutput(int numVertices, int numEdges, string opFile, string outputFinal, int contactSeq);
    vector<tuple<int, int, int>> adjustSlowIntervals(std::vector<tuple<int, int, int> > &intervalVector);
};

class compareTuple {
public:
    bool operator () (tuple<int, int, int, int> tup1, tuple<int, int, int, int> tup2)
    {
        if (get<2>(tup1) != get<2>(tup2))
            return (get<2>(tup1) < get<2>(tup2));
        else if (get<0>(tup1) != get<0>(tup2))
            return (get<0>(tup1) < get<0>(tup2));
        else if (get<1>(tup1) != get<1>(tup2))
            return (get<1>(tup1) < get<1>(tup2));
        else
            return (get<3>(tup1) < get<3>(tup2));
    }
};





#endif /* Graph_hpp */
