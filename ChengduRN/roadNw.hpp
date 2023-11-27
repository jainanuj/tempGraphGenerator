//
//  roadNw.hpp
//  temporalGraphGenerator
//
//  Created by Anuj Jain on 3/21/21.
//

#ifndef roadNw_hpp
#define roadNw_hpp

#include <stdio.h>
#include <vector>
using namespace std;

struct intvl {
    int startTime;
    int endTime;
    int trvlTime;
};
struct edges {
    int linkId;
    int u;
    int v;
    double length;
    int numIntvls;
    vector<intvl> intvls;
};
struct raw2mSpeed {
    int linkId;
    int durationStart;
    double speed;
};

class rnParser
{
public:
    vector<edges> rdNetwork;
    vector<vector<raw2mSpeed>> vecLinkSpeeeds;
    int numNodes; int numEdges;
    void readLinks(const char* fileName);
    void readSpeeds(const char* speedFile0, const char* speedFile1, int numLinks);
    void formIntvlsForLink(int linkId);
    void printIntvlGraph();
    bool withinXpct(int intvlTime, int newTime);
    void formIntvls();
    void printRnGraph(const char* fileName);
};


#endif /* roadNw_hpp */
