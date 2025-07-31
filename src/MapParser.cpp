#include "MapParser.h"
#include <fstream>
#include <sstream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <cmath>
using namespace std;

// Loads node positions: id -> (lat, lon)
unordered_map<int, pair<double, double>> MapParser::loadNodePositions(const string& filename) {
    unordered_map<int, pair<double, double>> nodePositions;
    ifstream file(filename);
    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        stringstream ss(line);
        int id;
        double lat, lon;
        char comma;
        ss >> id >> comma >> lat >> comma >> lon;
        nodePositions[id] = {lat, lon};
    }
    return nodePositions;
}

// Haversine formula (distance in meters)
double haversine(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371000.0; // meters
    double phi1 = lat1 * M_PI / 180.0, phi2 = lat2 * M_PI / 180.0;
    double dphi = (lat2 - lat1) * M_PI / 180.0;
    double dlambda = (lon2 - lon1) * M_PI / 180.0;
    double a = sin(dphi/2)*sin(dphi/2) + cos(phi1)*cos(phi2)*sin(dlambda/2)*sin(dlambda/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    return R * c;
}



// Loads the edge list, computes real weights, and adds to the graph
void MapParser::loadGraphWithRealWeights(const string& edgeFile,
                                         const string& nodeFile,
                                         Graph& graph) {
    auto nodePositions = MapParser::loadNodePositions(nodeFile);
    ifstream file(edgeFile);
    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        stringstream ss(line);
        int u, v;
        char comma;
        ss >> u >> comma >> v;
        if (nodePositions.count(u) && nodePositions.count(v)) {
            auto [lat1, lon1] = nodePositions[u];
            auto [lat2, lon2] = nodePositions[v];
            double dist = haversine(lat1, lon1, lat2, lon2);
            graph.addEdge(u, v, dist);
        }
    }
}

