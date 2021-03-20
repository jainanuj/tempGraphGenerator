//
//  Graph.cpp
//  temporalGraphGenerator
//
//  Created by Anuj Jain on 1/10/21.
//

#include "Graph.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <time.h>

#define MAX_RANGE 500
#define STD_DEV 2
#define AVG_NUM_INTVLS 20
#define AVG_INTVL_DURATION 8
#define AVG_TRVL_TIME 5

#define INTERMDT_OPEND "_op.txt"
#define FINAL_OPEND "_final.txt"
#define WU_END      "_wu.txt"

#define CONTACTSEQ_END "_cs_final.txt"



Graph::Graph(const char* filePath, int contactSeq = 0)      //Reads the edges of the graph in interval format. Edges will be specified as u, v, intvlCount, [intvls]. Each intvl is specified as (start, end, travelTime). End is last time instant at which edge can be used.
{
    //FILE* inFile;
    FILE* outFile;
    int x;
    int u, v, lambda, intvlStart, intvlEnd, numEdgesWritten = 0, maxVertex = 0;
//    vector<tuple<int, int, int>> intvls;
    std::default_random_engine defaultGenerator;
/*    std::default_random_engine numIntvlsGenerator;
    std::default_random_engine intvlDurationGenerator;
    std::default_random_engine travelTimeGenerator;*/
    std::normal_distribution<double> numIntvlsDistr(AVG_NUM_INTVLS, STD_DEV);
    std::normal_distribution<double> intvlDurationDistr(AVG_INTVL_DURATION, STD_DEV);
    std::normal_distribution<double> travelTimeDistr(AVG_TRVL_TIME, STD_DEV);

    string outputEnd = INTERMDT_OPEND;
    unsigned long len = outputEnd.length();
    std::string opFile(filePath);
    opFile.replace(opFile.length()-4, len, outputEnd);
//    cout << opFile;
    outFile = fopen(opFile.c_str(), "w");
    ifstream inputFile(filePath);
    string inputLine;
    srand( (int)time(NULL));
    
    while (getline(inputFile, inputLine))
    {
        x = sscanf(inputLine.c_str(), "%d %d", &u, &v);
        int numIntvls = (int)numIntvlsDistr(defaultGenerator);     //Use Gaussian distribution with (AVG_NUM_INTVLS, STD_DEV)
        while (numIntvls <= 0)
            numIntvls = (int)numIntvlsDistr(defaultGenerator);     //Use Gaussian distribution with (AVG_NUM_INTVLS, STD_DEV)
        int partitionSize = MAX_RANGE/numIntvls;
        
        fprintf(outFile, "%d %d %d ", u, v, numIntvls);
        int intvlDuration;
        for (int p = 0; p < numIntvls; p++)
        {
            if (contactSeq == 0)
            {
                intvlDuration = (int)intvlDurationDistr(defaultGenerator);     //Use gaussiona distr with (AVG_INTVL_DURATION, STD_DEV)
                while ( (intvlDuration <= 0) || ( (partitionSize*(p+1) - intvlDuration) < partitionSize*p) )
                    intvlDuration = intvlDurationDistr(defaultGenerator);     //Use gaussiona distr with (AVG_INTVL_DURATION, STD_DEV)
            }
            else
                intvlDuration = 0;
            intvlStart = rand() % (partitionSize - intvlDuration) + partitionSize*p;       //random number between these numbers.
            intvlEnd = intvlStart + intvlDuration;
            lambda = (int)travelTimeDistr(defaultGenerator);           //Use gaussian distr with (AVG_TRVL_TIME, STD_DEV)
            while (lambda <= 0)
                lambda = (int)travelTimeDistr(defaultGenerator);           //Use gaussian distr with (AVG_TRVL_TIME, STD_DEV)
            if (contactSeq == 0)
                fprintf(outFile, " %d %d %d", intvlStart, intvlEnd, lambda);
            else
                fprintf(outFile, " %d %d", intvlStart, lambda);
        }
        fprintf(outFile, "\n");
//        intvls.clear();
        numEdgesWritten++;
        if (u > maxVertex)
            maxVertex = u;
        if (v > maxVertex)
            maxVertex = v;
    }
    fclose(outFile);
    
    if (contactSeq == 0)
        outputEnd = FINAL_OPEND;
    else
        outputEnd = CONTACTSEQ_END;
    len = outputEnd.length();
    std::string outputFinal(opFile);
    outputFinal.replace(opFile.length()-4, len, outputEnd);

    finalizeOutput(maxVertex+1, numEdgesWritten, opFile, outputFinal, contactSeq);
    cout << "NumEdges: " << numEdgesWritten << ". Num Vertices: " << maxVertex << ".\n";
    
    xuanToWuModel(outputFinal.c_str(), contactSeq);
    
/*    string opEdgesVerts = to_string(maxVertex);
    opEdgesVerts += " ";
    opEdgesVerts += to_string(numEdgesWritten);
    outFile = fopen(opFile.c_str(), "r+b");
    fseek(outFile, 0, SEEK_SET);
    fputs(opEdgesVerts.c_str(), outFile);
    fseek(outFile, opEdgesVerts.length(), SEEK_SET);
    fputs("\n", outFile);
    fclose(outFile);*/
}


vector<tuple<int, int, int>> Graph::adjustSlowIntervals(std::vector<tuple<int, int, int> > &intervalVector)
{
    int len = (int) intervalVector.size();
    int j = 0, s, e, d, s2, e2, d2, newE;
    vector<tuple<int, int, int>> newIntvlsVector;      //Interval vector is (s, e, d). s=start time. e=end time. d=travel time or lambda.
    newIntvlsVector.push_back(intervalVector[len-1]); j++;

    for (int i = len - 2; i >= 0; i--)
    {
        s = std::get<0>(intervalVector[i]); e = std::get<1>(intervalVector[i]); d = std::get<2>(intervalVector[i]);
        s2 = std::get<0>(newIntvlsVector[j-1]); e2 = std::get<1>(newIntvlsVector[j-1]); d2 = std::get<2>(newIntvlsVector[j-1]);
        if ((e + d) > (s2 + d2))
        {
            newE = s2 + d2 - d;
            if (newE >= s)      //push the adjusted interval as s, newE, d. Else skip this interval.
            {
                newIntvlsVector.push_back(std::make_tuple(s, newE, d)); j++;
            }
        }
        else
        {
            newIntvlsVector.push_back(intervalVector[i]); j++;
        }
    }
    return newIntvlsVector;
}


void Graph::finalizeOutput(int numVertices, int numEdges, string opFile, string outputFinal, int contactSeq)
{
    vector <tuple<int, int, int>> inputIntvlsVector;
    int u, v, numIntvls, intvlStart, intvlEnd, lambda;

    ofstream outFile(outputFinal);
    outFile << numVertices << " " << numEdges << "\n";
    ifstream inputFile(opFile);
    string inputLine;
    
    while (getline(inputFile, inputLine))
    {
        stringstream parseLine(inputLine.c_str());
        parseLine >> u >> v >> numIntvls;
        outFile << u << "  " << v << "  ";
        inputIntvlsVector.clear();
        for (int intvlRead = 0; intvlRead <  numIntvls; intvlRead++)
        {
            if (contactSeq == 0)
            {
                parseLine >> intvlStart >> intvlEnd >> lambda;
                inputIntvlsVector.push_back(make_tuple(intvlStart, intvlEnd, lambda));
            }
            else
            {
                parseLine >> intvlStart >> lambda;
                inputIntvlsVector.push_back(make_tuple(intvlStart, intvlStart, lambda));
            }
        }
        vector <tuple<int, int, int>> outputIntvls = adjustSlowIntervals(inputIntvlsVector);
        outFile << outputIntvls.size() << "  ";     //OutputNumIntvls
        for (int i = (int)outputIntvls.size() - 1; i >= 0; i--)
        {
            if (contactSeq == 0)
                outFile << get<0>(outputIntvls[i]) << " " << get<1>(outputIntvls[i]) << " " << get<2>(outputIntvls[i]) << " ";
            else
                outFile << get<0>(outputIntvls[i]) << " " << get<2>(outputIntvls[i]) << " ";
        }
        outFile << "\n";
    }
}

void Graph::xuanToWuModel(const char* filePath, int contactSeq) // input file
{
    int u, v, lambda, numIntvls, intvlStart, intvlEnd, numEdgesWritten = 0, numVertices;
    string outputEnd = "_wu.txt";
    unsigned long len = outputEnd.length();
    std::string opFile(filePath);
    opFile.replace(opFile.length()-4, len, outputEnd);   //cout << opFile;
    ifstream inputFile(filePath);
    string inputLine;
    vector<tuple<int, int, int, int>> wuEdges;
    compareTuple tupleComp;

    getline(inputFile, inputLine);
    {
        stringstream parseFirst(inputLine.c_str());
        parseFirst >> numVertices;
    }
    
    while (getline(inputFile, inputLine))
    {
        stringstream parseLine(inputLine.c_str());
        parseLine >> u >> v >> numIntvls;
        for (int intvlRead = 0; intvlRead <  numIntvls; intvlRead++)
        {
            if (contactSeq == 0)
            {
                parseLine >> intvlStart >> intvlEnd >> lambda;
                for (int t = intvlStart; t <= intvlEnd; t++)
                {
                    wuEdges.push_back(make_tuple(u, v, t, lambda));
                    //fprintf(outFile, "%d %d %d %d\n", u, v, t, lambda);
                    numEdgesWritten++;
                }
            }
            else
            {
                parseLine >> intvlStart >> lambda;
                int t = intvlStart;
                wuEdges.push_back(make_tuple(u, v, t, lambda));
                numEdgesWritten++;
            }
        }
    }
    sort<vector<tuple<int, int, int, int>>::iterator, compareTuple>(wuEdges.begin(), wuEdges.end(), tupleComp);

    FILE* outFile = fopen(opFile.c_str(), "w");
    fprintf(outFile, "%d %d\n", numVertices, numEdgesWritten);
    for (int i = 0; i < wuEdges.size(); i++)
    {
        fprintf(outFile, "%d %d %d %d\n", get<0>(wuEdges[i]), get<1>(wuEdges[i]), get<2>(wuEdges[i]), get<3>(wuEdges[i]));
    }
    fclose(outFile);
    cout << "Num of Wu Edges: " << numEdgesWritten << "\n";
}


