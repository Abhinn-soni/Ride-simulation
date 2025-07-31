#pragma once
#include <unordered_map>
#include <vector>
#include <utility>

// Each node is an integer ID.
// Each edge has a destination (int) and a weight (double, say distance)

class Graph {
public:
    void addEdge(int u, int v, double w);  // Add a road between u and v with distance w
    const std::vector<std::pair<int, double>>& getNeighbors(int node) const;

private:
    std::unordered_map<int, std::vector<std::pair<int, double>>> adjList;
    // Key: Node ID, Value: List of roads (neighbor node ID, weight)
};
