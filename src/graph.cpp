#include "Graph.h"

void Graph::addEdge(int u, int v, double w) {
    adjList[u].emplace_back(v, w);
    adjList[v].emplace_back(u, w); // Both ways (roads work in both directions)
}

const std::vector<std::pair<int, double>>& Graph::getNeighbors(int node) const {
    static const std::vector<std::pair<int, double>> empty;
    auto it = adjList.find(node);
    if (it != adjList.end()) return it->second;
    else return empty;
}
