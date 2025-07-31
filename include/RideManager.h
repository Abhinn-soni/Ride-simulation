#pragma once
#include "Graph.h"
#include "Driver.h"
#include "Rider.h"
#include <vector>

class RideManager {
public:
    RideManager(Graph& g);

    // Finds the shortest path between two nodes (returns the path as a list of node IDs)
    std::vector<int> findShortestPath(int src, int dest);

    // Finds the available driver with shortest-path (returns driver's vector index, or -1 if none)
    int matchRiderToDriver(const Rider& rider, const std::vector<Driver>& drivers);

private:
    Graph& graph;
};
