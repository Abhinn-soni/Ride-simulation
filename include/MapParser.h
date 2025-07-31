#pragma once
#include "Graph.h"
#include <string>
#include <unordered_map>
#include <utility>

class MapParser {
public:
    // Loads node positions (id → (lat, lon))
    static std::unordered_map<int, std::pair<double, double>> loadNodePositions(const std::string& filename);

    // Loads the graph using real distances for edge weights
    static void loadGraphWithRealWeights(const std::string& edgeFile,
                                         const std::string& nodeFile,
                                         Graph& graph);
};
