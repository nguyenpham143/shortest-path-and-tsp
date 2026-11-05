#include "tsm.h"

// Lay tap hop dinh duy nhat va sap xep theo ASCII
vector<char> getVertices(int edgeList[][3], int numEdges) {
    bool appeared[1000] = {false};
    vector<char> vertices;

    for (int i = 0; i < numEdges; i++) {
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
    for (int i = 0; i < vertices.size(); i++)
        if (vertices[i] == c) {
            return i;
        }
    return -1;
}

string Traveling(int edgeList[][3], int numEdges, char startVertex) {
    vector<char> vertices = getVertices(edgeList, numEdges);
    int numVertices = vertices.size();
    int startIndex = getVertexIndex(vertices, startVertex);

    if (numVertices == 0 || startIndex == -1) {
        return "";
    }

    // Bellman-Held-Karp cho numVertices <= 20
    else if (numVertices > 0 && numVertices <= 20) {
        // Tao ma tran trong so
        vector<vector<int>> weight(numVertices, vector<int>(numVertices, -1));
        for (int i = 0; i < numEdges; i++) {
            int u = getVertexIndex(vertices, (char)edgeList[i][0]);
            int v = getVertexIndex(vertices, (char)edgeList[i][1]);
            weight[u][v] = edgeList[i][2];
        }

        int subsetCount = 1 << numVertices;

        // dp[mask][u]: khoang cach nho nhat de di qua cac dinh trong mask va ket thuc tai u
        vector<vector<int>> dp(subsetCount, vector<int>(numVertices, -1));

        // prev[mask][u]: dinh truoc u trong hanh trinh mask tuong ung -> dung de truy vet
        vector<vector<int>> prev(subsetCount, vector<int>(numVertices, -1));

        // Trang thai ban dau
        dp[1 << startIndex][startIndex] = 0;

        // Duyet tat ca cac trang thai tap con mask
        for (int mask = 0; mask < subsetCount; mask++) {

            // Duyet tat ca cac dinh u xem co ket thuc tai u trong trang thai mask hay khong
            for (int u = 0; u < numVertices; u++) {
                if (dp[mask][u] == -1) continue;
                
                // Duyet tat ca cac dinh v de thu mo rong tu u->v
                for (int v = 0; v < numVertices; v++) {
                    // Bo qua neu da di qua v hoac khong co canh tu u->v
                    if ((mask >> v) & 1 || weight[u][v] == -1) continue;

                    // Tao trang thai mask moi sau khi them v vao mask hien tai
                    int nextMask = mask | (1 << v);

                    // Tinh tong khoang cach moi khi di tu u->v
                    int newDist = dp[mask][u] + weight[u][v];

                    // Cap nhat khoang cach toi uu va u la dinh truoc v 
                    if (dp[nextMask][v] == -1 || newDist < dp[nextMask][v]) {
                        dp[nextMask][v] = newDist;
                        prev[nextMask][v] = u;
                    }
                }
            }
        }

        // Tim diem ket thuc hanh trinh
        // fullMask = bitmask co tat ca cac dinh da duoc di qua
        int fullMask = (1 << numVertices) - 1;

        // last luu lai index cua dinh cuoi cung truoc khi quay ve diem xuat phat
        // minCost la tong khoang cach nho nhat thoa man yeu cau
        int last = -1;
        int minCost = -1;

        // Duyet tat ca dinh u xem dinh nao la diem ket thuc tot nhat
        for (int u = 0; u < numVertices; u++) {
            // Bo qua neu u chinh la diem bat dau hoac khong co duong di qua tat ca cac dinh va ket thuc o u
            // hoac khong co canh tu u quay lai startVertex
            if (u == startIndex || dp[fullMask][u] == -1 || weight[u][startIndex] == -1) continue;

            // Neu la dinh dau tien duoc chon 
            // hoac tong khoang cach tu start di qua tat ca dinh ket thuc o u roi quay lai start la nho nhat
            int cost = dp[fullMask][u] + weight[u][startIndex];
            if (minCost == -1 || cost < minCost || (cost == minCost && u > last)) {
                minCost = cost;
                last = u;
            }
        }

        // Neu khong co chu trinh Hamilton
        if (last == -1) {
            return "";
        }

        // Truy vet
        vector<int> path;
        int mask = fullMask;
        int current = last;
        while (current != -1) {
            path.push_back(current);
            int temp = prev[mask][current];  // Lay ra dinh lien truoc current trong hanh trinh toi uu
            mask ^= (1 << current);          // Xoa dinh current khoi tap bitmask
            current = temp;                  // Di chuyen nguoc lai den dinh truoc
        }

        reverse(path.begin(), path.end());

        // Ghep string
        string result = "";
        for (int i = 0; i < path.size(); i++) {
            result += vertices[path[i]];
            if (i < path.size()) result += " ";
        }
        result += startVertex;

        return result;
    }

    // Ant-Colony-Optimization cho numVertices > 20
    else if (numVertices > 20) {
        // Cac tham so hang so cho ACO
        const int N_EPOCH = 200;               // So vong lap (epoch)
        const int NUM_ANTS = 100;              // So kien moi epoch
        const double ALPHA = 0.5;              // Trong so pheromone
        const double BETA = 3.0;               // Trong so heuristic
        const double RHO = 0.1;                // Ty le bay hoi pheromone
        const double Q = 100.0;                // Hang so quy doi pheromone
        const double INITIAL_PHEROMONE = 0.1;  // Gia tri pheromone ban dau

        // Bo tao so ngau nhien cho ACO
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0.0, 1.0);

        // Khoi tao ma tran khoang cach tu edgeList
        vector<vector<int>> distance(numVertices, vector<int>(numVertices, 0));
        for (int i = 0; i < numEdges; i++) {
            int u = getVertexIndex(vertices, (char)edgeList[i][0]);
            int v = getVertexIndex(vertices, (char)edgeList[i][1]);
            distance[u][v] = edgeList[i][2];
        }

        // Khoi tao ma tran pheromone va heuristic
        vector<vector<double>> pheromone(numVertices, vector<double>(numVertices, 0.0));
        vector<vector<double>> heuristic(numVertices, vector<double>(numVertices, 0.0));
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                if (distance[i][j] > 0) {
                    pheromone[i][j] = INITIAL_PHEROMONE;   // Pheromone ban dau cho cac canh hop le
                    heuristic[i][j] = 1.0 / distance[i][j]; // Heuristic = 1/khoang_cach
                }
            }
        }

        // Luu hanh trinh tot nhat va do dai tuong ung
        vector<int> best_tour;
        double best_length = -1;

        // Vong lap ACO chinh
        for (int epoch = 0; epoch < N_EPOCH; epoch++) {
            // Luu cac tours va lengths cho tat ca kien trong epoch nay
            vector<vector<int>> ant_tours(NUM_ANTS);
            vector<double> ant_lengths(NUM_ANTS, -1);

            // Moi kien xay dung 1 tour
            for (int k = 0; k < NUM_ANTS; k++) {
                // Cac dinh da tham va tour cua kien hien tai
                vector<bool> visited(numVertices, false);
                vector<int> tour;
                tour.push_back(startIndex);
                visited[startIndex] = true;

                // Xay dung duong di cho den khi tham het cac dinh
                while (tour.size() < numVertices) {
                    int current = tour.back(); 
                    double prob_sum = 0.0; // Tong xac suat cho cac thanh pho tiep theo
                    vector<pair<double, int>> candidates; // Luu xac suat va chi so thanh pho

                    // Tinh xac suat cho cac thanh pho chua tham co canh hop le
                    for (int j = 0; j < numVertices; j++) {
                        if (!visited[j] && distance[current][j] > 0) {
                            double prob = pow(pheromone[current][j], ALPHA) * pow(heuristic[current][j], BETA);
                            prob_sum += prob;
                            candidates.emplace_back(prob, j);
                        }
                    }

                    // Chon thanh pho tiep theo bang roulette wheel selection
                    if (prob_sum > 0) {
                        double r = dis(gen) * prob_sum; // Gia tri ngau nhien trong [0, prob_sum)
                        double cumulative = 0.0;
                        for (const auto& candidate : candidates) {
                            cumulative += candidate.first;
                            if (cumulative >= r) {
                                tour.push_back(candidate.second);   // Them thanh pho vao tour
                                visited[candidate.second] = true;
                                break;
                            }
                        }
                    } else {
                        break;
                    }
                }

                // Kiem tra tour hop le va tinh do dai
                if (tour.size() == numVertices && distance[tour.back()][startIndex] > 0) {
                    double length = 0.0;
                    for (int i = 0; i < tour.size() - 1; i++) {
                        length += distance[tour[i]][tour[i + 1]];
                    }
                    length += distance[tour.back()][startIndex]; // Them canh quay ve
                    ant_lengths[k] = length;
                    ant_tours[k] = tour;
                    // Cap nhat best_tour neu ngan hon
                    if (best_length < 0 || length < best_length) {
                        best_length = length;
                        best_tour = tour;
                    }
                }
            }

            // Bay hoi pheromone tren tat ca cac canh
            for (int i = 0; i < numVertices; i++) {
                for (int j = 0; j < numVertices; j++) {
                    if (distance[i][j] > 0) {
                        pheromone[i][j] *= (1 - RHO);  // Luong bay hoi theo RHO
                    }
                }
            }

            // Nap pheromone cho cac duong di hop le
            for (int k = 0; k < NUM_ANTS; k++) {
                if (ant_lengths[k] > 0) {
                    double deposit = Q / ant_lengths[k]; // Luong pheromone lang dong
                    const auto& tour = ant_tours[k];
                    for (int i = 0; i < tour.size() - 1; i++) {
                        int u = tour[i];
                        int v = tour[i + 1];
                        pheromone[u][v] += deposit;
                    }
                    pheromone[tour.back()][startIndex] += deposit;
                }
            }
        }

        // Ghep string tu best_tour
        if (best_tour.empty()) return "";
        string result = "";
        for (int i = 0; i < best_tour.size(); i++) {
            result += vertices[best_tour[i]];
            if (i < best_tour.size()) result += " ";
        }
        result += startVertex;

        return result;
    }

    return "";
}
