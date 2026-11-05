#ifndef _BELLMAN_H_
#define _BELLMAN_H_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

vector<char> getVertices(int edgeList[][3], int numEdges);
int getVertexIndex(const vector<char>& vertices, char c);
bool compareEdges(const vector<int>& a, const vector<int>& b);
void BF(int edgeList[][3], int numEdges, char startVertex, int BFvalue[], int BFprev[]);
string BF_Path(int edgeList[][3], int numEdges, char startVertex, char goalVertex);

#endif