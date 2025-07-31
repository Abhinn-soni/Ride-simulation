#pragma once
#include <string>

class Logger {
public:
    static void logRide(int riderId, int driverId, int pickupNode, int dropoffNode,
                        double pickupDist, double rideDist);

    static void logNoMatch(int riderId);

    static void logRideToFile(int riderId, int driverId, int pickupNode, int dropoffNode,
                              double pickupDist, double rideDist, 
                              const std::string& pathToRider, const std::string& pathToDestination);

    static void printLogFile(const std::string& filename);
};
