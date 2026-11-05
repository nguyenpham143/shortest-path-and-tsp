#ifndef _TSM_H_
#define _TSM_H_

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

vector<char> getVertices(int edgeList[][3], int numEdges);
int getVertexIndex(const vector<char>& vertices, char c);
string Traveling(int edgeList[][3], int numEdges, char startVertex);

#endif