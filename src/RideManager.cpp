#include "RideManager.h"
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>
using namespace std;

// Constructor
RideManager::RideManager(Graph& g) : graph(g) {}

// Dijkstra's algorithm for shortest path from src to dest
vector<int> RideManager::findShortestPath(int src, int dest) {
    unordered_map<int, double> dist;
    unordered_map<int, int> prev;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;

    dist[src] = 0.0;
    pq.push({0.0, src});

    while (!pq.empty()) {
        auto [curDist, u] = pq.top(); pq.pop();
        if (u == dest) break;
        if (curDist > dist[u]) continue;
        for (const auto& [v, w] : graph.getNeighbors(u)) {
            double alt = curDist + w;
            if (!dist.count(v) || alt < dist[v]) {
                dist[v] = alt;
                prev[v] = u;
                pq.push({alt, v});
            }
        }
    }
    // Reconstruct path
    vector<int> path;
    for (int at = dest; prev.count(at); at = prev[at])
        path.push_back(at);
    if (src == dest || !path.empty()) path.push_back(src);
    reverse(path.begin(), path.end());
    return path;
}

// Finds the available driver with the shortest path to rider
int RideManager::matchRiderToDriver(const Rider& rider, const vector<Driver>& drivers) {
    int bestDriverIndex = -1;
    double minDist = numeric_limits<double>::max();

    for (size_t i = 0; i < drivers.size(); ++i) {
        if (drivers[i].status == DriverStatus::Available) {
            vector<int> path = findShortestPath(drivers[i].location, rider.location);
            if (path.size() < 2) continue; // No valid path

            // Calculate path total distance
            double totalDist = 0.0;
            for (size_t j = 1; j < path.size(); ++j) {
                for (const auto& nei : graph.getNeighbors(path[j-1])) {
                    if (nei.first == path[j]) {
                        totalDist += nei.second;
                        break;
                    }
                }
            }
            if (totalDist < minDist) {
                minDist = totalDist;
                bestDriverIndex = static_cast<int>(i);
            }
        }
    }
    return bestDriverIndex;
}
