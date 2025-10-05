#include "bellman.h"

// Lay tap hop dinh duy nhat va sap xep theo ASCII
vector<char> getVertices(int edgeList[][3], int numEdges) {
    bool appeared[1000] = {false};
    vector<char> vertices;

    for (int i = 0; i < numEdges; ++i) {
        char u = (char)edgeList[i][0];
        char v = (char)edgeList[i][1];
        if (!appeared[(int)u]) {
            appeared[(int)u] = true;
            vertices.push_back(u);
        }
        if (!appeared[(int)v]) {
            appeared[(int)v] = true;
            vertices.push_back(v);
        }
    }

    sort(vertices.begin(), vertices.end());
    return vertices;
}

// Lay index cua c
int getVertexIndex(const vector<char>& vertices, char c) {
    for (int i = 0; i < vertices.size(); i++) {
        if (vertices[i] == c) {
            return i;
        }
    }

    return -1;
}

// Ham so sanh de sap xep canh theo chi so dinh nguon tang dan
bool compareEdges(const vector<int>& a, const vector<int>& b) {
    return a[0] < b[0];
}

void BF(int edgeList[][3], int numEdges, char startVertex, int BFvalue[], int BFprev[]) {
    vector<char> vertices = getVertices(edgeList, numEdges);
    int numVertices = vertices.size();

    // Kiem tra xem co phai lan goi dau tien
    bool firstCall = true;
    for (int i = 0; i < numVertices; i++) {
        if (BFvalue[i] != -1) {
            firstCall = false;
            break;
        }
    }

    // Dat BFvalue[startIndex] = 0 neu la lan goi dau tien
    int startIndex = getVertexIndex(vertices, startVertex);
    if (firstCall) {
        if (startIndex != -1) {
            BFvalue[startIndex] = 0;
        }
    }

    // Tao tempBFvalue de tinh toan
    int tempBFvalue[1000];
    for (int i = 0; i < numVertices; i++) {
        tempBFvalue[i] = BFvalue[i];
    }

    // Tao danh sach canh va sap xep tang dan Index
    vector<vector<int>> edges;
    for (int i = 0; i < numEdges; i++) {
        char u = (char)edgeList[i][0];
        char v = (char)edgeList[i][1];
        int w = edgeList[i][2];

        int from = getVertexIndex(vertices, u);
        int to = getVertexIndex(vertices, v);

        if (from != -1 && to != -1) {
            edges.push_back({from, to, w});
        }
    }
    sort(edges.begin(), edges.end(), compareEdges);

    // Thuc hien 1 step Bellman-Ford
    for (const auto& edge : edges) {
        int from = edge[0];
        int to = edge[1];
        int w = edge[2];

        if (tempBFvalue[from] != -1) {
            if (BFvalue[to] == -1 || tempBFvalue[from] + w < BFvalue[to]) {
                BFvalue[to] = tempBFvalue[from] + w;
                BFprev[to] = from;
            }
        }
    }
}

string BF_Path(int edgeList[][3], int numEdges, char startVertex, char goalVertex) {
    vector<char> vertices = getVertices(edgeList, numEdges);
    int numVertices = vertices.size();
    int BFvalue[1000];
    int BFprev[1000];

    // Khoi tao mang
    for (int i = 0; i < numVertices; i++) {
        BFvalue[i] = -1;
        BFprev[i] = -1;
    }

    // Dat BFvalue[startIndex] = 0
    int startIndex = getVertexIndex(vertices, startVertex);
    if (startIndex != -1) {
        BFvalue[startIndex] = 0;
    }

    // Tao danh sach canh 1 lan va sap xep tang dan Index
    vector<vector<int>> edges;
    for (int i = 0; i < numEdges; i++) {
        char u = (char)edgeList[i][0];
        char v = (char)edgeList[i][1];
        int w = edgeList[i][2];

        int from = getVertexIndex(vertices, u);
        int to = getVertexIndex(vertices, v);

        if (from != -1 && to != -1) {
            edges.push_back({from, to, w});
        }
    }
    sort(edges.begin(), edges.end(), compareEdges);

    // Thuc hien Bellman-Ford numVertices - 1 lan
    for (int step = 0; step < numVertices - 1; step++) {
        int tempBFvalue[1000];
        for (int i = 0; i < numVertices; i++) {
            tempBFvalue[i] = BFvalue[i];
        }
        
        for (const auto& edge : edges) {
            int from = edge[0];
            int to = edge[1];
            int w = edge[2];

            if (tempBFvalue[from] != -1) {
                if (BFvalue[to] == -1 || tempBFvalue[from] + w < BFvalue[to]) {
                    BFvalue[to] = tempBFvalue[from] + w;
                    BFprev[to] = from;
                }
            }
        }
    }

    // Backtracking
    int goalIndex = getVertexIndex(vertices, goalVertex);
    if (BFvalue[goalIndex] == -1) {
        return "";
    }

    vector<char> path;
    int current = goalIndex;
    while (current != -1) {
        path.push_back(vertices[current]);
        if (vertices[current] == startVertex) {
            break;
        }
        current = BFprev[current];
    }
    
    reverse(path.begin(), path.end());

    // Ghep string
    string result = "";
    for (int i = 0; i < path.size(); i++) {
        result += path[i];
        if (i < path.size() - 1) {
            result += " ";
        }
    }

    return result;
}
