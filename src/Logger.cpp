#include "Logger.h"
#include <iostream>
#include <fstream>
using namespace std;

void Logger::logRide(int riderId, int driverId, int pickupNode, int dropoffNode,
                     double pickupDist, double rideDist) {
    cout << "Rider " << riderId << " matched to Driver " << driverId << endl;
    cout << "  Pickup node: " << pickupNode << ", Dropoff node: " << dropoffNode << endl;
    cout << "  Driver-to-rider distance: " << pickupDist << " meters" << endl;
    cout << "  Rider trip distance: " << rideDist << " meters" << endl;
    cout << "  Total trip: " << (pickupDist + rideDist) << " meters" << endl << endl;
}

void Logger::logNoMatch(int riderId) {
    cout << "Rider " << riderId << ": No available driver can reach the rider." << endl << endl;
}

void Logger::logRideToFile(int riderId, int driverId, int pickupNode, int dropoffNode,
                           double pickupDist, double rideDist, 
                           const string& pathToRider, const string& pathToDestination) {
    ofstream ofs("ride_log.txt", ios::app);
    if (!ofs) {
        cerr << "Error opening log file for writing." << endl;
        return;
    }
    ofs << "Rider " << riderId << " matched to Driver " << driverId << "\n";
    ofs << "  Pickup node: " << pickupNode << ", Dropoff node: " << dropoffNode << "\n";
    ofs << "  Driver-to-rider distance: " << pickupDist << " meters\n";
    ofs << "  Rider trip distance: " << rideDist << " meters\n";
    ofs << "  Total trip: " << (pickupDist + rideDist) << " meters\n";
    ofs << "  Path followed by driver to reach rider: " << pathToRider << "\n";
    ofs << "  Path from pickup to destination: " << pathToDestination << "\n";
    ofs << "---------------------------------------------\n";
}

void Logger::printLogFile(const string& filename) {
    ifstream ifs(filename);
    if (!ifs) {
        cerr << "Error opening log file for reading." << endl;
        return;
    }
    string line;
    while (getline(ifs, line)) {
        cout << line << endl;
    }
}
