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
//#include "roadNw.hpp"

int main(int argc, const char * argv[]) {
    Graph g;
    int contactSeq = 0;
    if (argc == 6)
        contactSeq = 1;
    if (argc <5)
        g = Graph(argv[1]);
    else
    {
        int avgNumIntvls = atoi(argv[2]), avgIntvlDuration=atoi(argv[3]), avgTrvlTime=atoi(argv[4]);
        if (argc ==5)
            g = Graph(argv[1], avgNumIntvls, avgIntvlDuration, avgTrvlTime);
        else
            g = Graph(argv[1], avgNumIntvls, avgIntvlDuration, avgTrvlTime,contactSeq);
    }
/*    rnParser roadNw;
    roadNw.readLinks(argv[1]);
    roadNw.readSpeeds(argv[2], argv[3], (int)roadNw.rdNetwork.size());
    roadNw.formIntvls();
    roadNw.printRnGraph(argv[1]);
*/
    std::cout << "Hello, World!\n";
    return 0;
}
