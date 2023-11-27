//
//  roadNw.cpp
//  temporalGraphGenerator
//
//  Created by Anuj Jain on 3/21/21.
//

#include "roadNw.hpp"
#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#define DURATIONS_PER_FILE 150
#define XPCT 0.05
#define RNGRAPHFN "_rnGraph.txt"

void rnParser::readLinks(const char *fileName)
{
    ifstream linksFile(fileName);
    string inputLine;
    int linkId = 0;
    edges currentEdge;
    string token;
    int maxNode = 0;

    getline(linksFile, inputLine);                      //skip header.
    while (getline(linksFile, inputLine))
    {
        istringstream linksDesc(inputLine);             //link desc is: linkId, u, longitdue, latittude, v, longtude, lattude, length.
        
        getline(linksDesc, token, ',');
        currentEdge.linkId = stoi(token);

        getline(linksDesc, token, ',');
        currentEdge.u = stoi(token);
        if ((currentEdge.u) > maxNode)
            maxNode = currentEdge.u;

        getline(linksDesc, token, ',');     //discard longtude
        getline(linksDesc, token, ',');     //discard lattude

        getline(linksDesc, token, ',');
        currentEdge.v = stoi(token);
        if ((currentEdge.v) > maxNode)
            maxNode = currentEdge.v;

        getline(linksDesc, token, ',');
        getline(linksDesc, token, ',');
        
        getline(linksDesc, token, ',');
        currentEdge.length = stod(token);
        
        rdNetwork.push_back(currentEdge);
        linkId++;
    }
    numNodes = maxNode;
    numEdges = linkId;
    assert(linkId == (int)rdNetwork.size());
}

void rnParser::readSpeeds(const char *speedFile0, const char *speedFile1, int numLinks)
{
    int rowNum = 0;
    vecLinkSpeeeds.resize(numLinks);

    string inputLine;
    string token;
    stringstream tokenConverter;
    raw2mSpeed speedSample;
    ifstream spdFile(speedFile0);
    getline(spdFile, inputLine);                    //skip header.
    while (getline(spdFile, inputLine))
    {
        istringstream spdDesc(inputLine);             //spdsample desc is: Period, linkId, speed.
        getline(spdDesc, token, ',');
        speedSample.durationStart = rowNum/numLinks;

        getline(spdDesc, token, ',');
        speedSample.linkId = stoi(token);

        getline(spdDesc, token, ',');
        speedSample.speed = stod(token);
        
        vecLinkSpeeeds[speedSample.linkId - 1].push_back(speedSample);
        rowNum++;
    }
    assert(rowNum == numLinks * DURATIONS_PER_FILE);

    rowNum = 0;
    ifstream spdFile1(speedFile1);
    getline(spdFile1, inputLine);                    //skip header.
    while (getline(spdFile1, inputLine))
    {
        istringstream spdDesc(inputLine);             //spdsample desc is: Period, linkId, speed.
        getline(spdDesc, token, ',');
        speedSample.durationStart = rowNum/numLinks + DURATIONS_PER_FILE;

        getline(spdDesc, token, ',');
        speedSample.linkId = stoi(token);

        getline(spdDesc, token, ',');
        speedSample.speed = stod(token);

        vecLinkSpeeeds[speedSample.linkId - 1].push_back(speedSample);
        rowNum++;
    }
    assert(rowNum == numLinks * DURATIONS_PER_FILE);
}

void rnParser::formIntvls()
{
    for (int i=0; i < rdNetwork.size(); i++)
    {
        formIntvlsForLink(i+1);
    }
}

void rnParser::printRnGraph(const char *fileName)
{
    string rnGraphEnd = RNGRAPHFN;
    unsigned long len = rnGraphEnd.length();
    std::string opFile(fileName);
    opFile.replace(opFile.length()-4, len, rnGraphEnd);
    ofstream outFile(opFile);
    outFile << numNodes << " " << numEdges << "\n";
    
    for (int i = 0; i < rdNetwork.size(); i++)
    {
        outFile << rdNetwork[i].u << "   " << rdNetwork[i].v << "   " << rdNetwork[i].numIntvls << "   ";
        for (int j = 0; j < rdNetwork[i].intvls.size(); j++)
        {
            outFile << rdNetwork[i].intvls[j].startTime << "  " << rdNetwork[i].intvls[j].endTime << "  " << rdNetwork[i].intvls[j].trvlTime << "   ";
        }
        outFile << "\n";
    }

}


void rnParser::formIntvlsForLink(int linkId)
{
    double distanceTravelled = 0; int trvlTime = 0; double distRemaining = 0; int additionalTime = 0;
    int startIntvl = 0, endIntvl = 0, intvlTravelTime = -1;
    int spdIndexStrt = 0, spdIndexEnd = 0;
    startIntvl = spdIndexStrt * 2 * 60;
    intvl newIntvl;
    rdNetwork[linkId-1].numIntvls = 0;
    int sumTrvlTimesInIntvl = 0;
    int numTrvlTimes = 0;
    while (spdIndexEnd < vecLinkSpeeeds[linkId-1].size())
    {
        while ( (distanceTravelled < rdNetwork[linkId-1].length) && (spdIndexEnd < vecLinkSpeeeds[linkId-1].size()) )
        {
            distanceTravelled += vecLinkSpeeeds[linkId-1][spdIndexEnd++].speed * 2;
            if (spdIndexEnd >= vecLinkSpeeeds[linkId-1].size())
                break;
        }
        spdIndexEnd--;
        additionalTime = 0;
        if ((distanceTravelled < rdNetwork[linkId-1].length) && (spdIndexEnd >= (vecLinkSpeeeds[linkId-1].size() - 1) ) )
            break;
        if (distanceTravelled > rdNetwork[linkId-1].length)
        {
            distanceTravelled -= vecLinkSpeeeds[linkId-1][spdIndexEnd].speed * 2;
            distRemaining  = rdNetwork[linkId-1].length - distanceTravelled;
            additionalTime = (distRemaining/vecLinkSpeeeds[linkId-1][spdIndexEnd].speed) *60;
        }
        trvlTime = (spdIndexEnd-spdIndexStrt) * 2 *60 + additionalTime;
        if (intvlTravelTime == -1)
        {
            intvlTravelTime = trvlTime;
            spdIndexStrt++;
            distanceTravelled = 0;
            spdIndexEnd = spdIndexStrt;
            sumTrvlTimesInIntvl = trvlTime;
            numTrvlTimes = 1;
        }
        else if (withinXpct(intvlTravelTime, trvlTime))  //If the trvlTime is within 10% of prev. travel time, extend the end time of the current interval to include the start time of this journey.
        {
            spdIndexStrt++;
            distanceTravelled = 0;
            spdIndexEnd = spdIndexStrt;
            sumTrvlTimesInIntvl += trvlTime;
            numTrvlTimes++;
        }
        else
        {
            endIntvl = (spdIndexStrt) * 2 * 60 - 1;
            newIntvl.startTime = startIntvl;
            newIntvl.endTime = endIntvl;
            newIntvl.trvlTime = sumTrvlTimesInIntvl/numTrvlTimes;
            rdNetwork[linkId-1].intvls.push_back(newIntvl);
            rdNetwork[linkId-1].numIntvls++;
            
            startIntvl = spdIndexStrt * 2 * 60;             //Start time of next intvl.
            //Start up counting times for the next intvl.
            intvlTravelTime = trvlTime;
            spdIndexStrt++;
            distanceTravelled = 0;
            spdIndexEnd = spdIndexStrt;
            sumTrvlTimesInIntvl = trvlTime;
            numTrvlTimes = 1;
        }
    }
    if (numTrvlTimes > 0)           //Close the last running intvl.
    {
        endIntvl = (spdIndexStrt-1) * 2 * 60 - 1;
        newIntvl.startTime = startIntvl;
        newIntvl.endTime = endIntvl;
        newIntvl.trvlTime = sumTrvlTimesInIntvl/numTrvlTimes;
        rdNetwork[linkId-1].intvls.push_back(newIntvl);
        rdNetwork[linkId-1].numIntvls++;
    }
}

bool rnParser::withinXpct(int intvlTime, int newTime)
{
    double timeDiff = 0;
    if (newTime > intvlTime)
        timeDiff = newTime - intvlTime;
    else
        timeDiff = intvlTime - newTime;
    if (timeDiff/(double)intvlTime < XPCT)
        return true;
    else
        return false;
}
