#include "Graph.h"
#include "Driver.h"
#include "Rider.h"
#include "Logger.h"
#include "RideManager.h"
#include "MapParser.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>  // for std::pair
#include <fstream>
#include <sstream>

using namespace std;

// Declare global or file-level nodePositions map
std::unordered_map<int, std::pair<double, double>> nodePositions;


// === Menu utility ===
void displayMenu() {
    cout << "\n===== Ride-Matching Simulation Menu =====\n";
    cout << "1. Add new driver\n";
    cout << "2. Add new rider\n";
    cout << "3. List all drivers\n";
    cout << "4. List all riders\n";
    cout << "5. Match next rider to driver\n";
    cout << "6. Show simulation summary\n";
    cout << "7. Remove a driver\n";
    cout << "8. Remove a rider\n";
    cout << "9. View ride logs\n";
    cout << "0. Exit\n";
    cout << "Enter option: ";
}

int main() {
    // Clear ride log file at each run
    ofstream ofs("ride_log.txt", ofstream::trunc); ofs.close();

    Graph g;
    // Load your map data (update the paths as needed)
    MapParser::loadGraphWithRealWeights("data/edges.csv", "data/nodes.csv", g);

    // Load node positions into nodePositions map
    nodePositions = MapParser::loadNodePositions("data/nodes.csv");

    // Optional: Prepare visualization CSV file and write CSV header
    std::ofstream csv_out("visualization_paths.csv", std::ofstream::trunc);
    csv_out << "RideID,PathOrder,NodeID,Lat,Lon\n";
    csv_out.close();


    vector<Driver> drivers{
        Driver(1, 319962734),
        Driver(2, 320144799),
        Driver(3, 320144584),
        Driver(4, 320144398),
        Driver(5, 320143674)
    };

    vector<Rider> riders{
        Rider(200, 319962734, 320144799),
        Rider(201, 320144799, 320144584),
        Rider(202, 320144584, 320144398),
        Rider(203, 320144398, 319963023),
        Rider(204, 319963023, 320143674)
    };

    RideManager manager(g);

    int choice;
    bool running = true;
    while (running) {
        displayMenu();
        cin >> choice;
        switch (choice) {

        // 1. Add new driver
        case 1: {
            int id, nodeId;
            cout << "Enter new driver ID: ";
            cin >> id;
            cout << "Enter driver start node ID: ";
            cin >> nodeId;
            drivers.emplace_back(id, nodeId);
            cout << "Driver added.\n";
            break;
        }

        // 2. Add new rider
        case 2: {
            int id, start, dest;
            cout << "Enter new rider ID: ";
            cin >> id;
            cout << "Enter rider start node ID: ";
            cin >> start;
            cout << "Enter rider destination node ID: ";
            cin >> dest;
            riders.emplace_back(id, start, dest);
            cout << "Rider added.\n";
            break;
        }

        // 3. List all drivers
        case 3: {
            cout << "\nCurrent Drivers:\n";
            for (const auto& d : drivers) {
                cout << "ID: " << d.id << ", Node: " << d.location << ", Status: ";
                if (d.status == DriverStatus::Available) cout << "Available";
                else if (d.status == DriverStatus::EnRoute) cout << "En Route";
                else cout << "Busy";
                cout << "\n";
            }
            break;
        }

        // 4. List all riders
        case 4: {
            cout << "\nCurrent Riders:\n";
            for (const auto& r : riders) {
                cout << "ID: " << r.id << ", Start: " << r.location << ", Destination: " << r.destination << "\n";
            }
            break;
        }




        // 5. Match next rider to driver AND log to file (not console)
       case 5: {
    if (riders.empty()) {
        cout << "No riders waiting for matching.\n";
        break;
    }
    Rider& rider = riders.front();

    int matchedIndex = manager.matchRiderToDriver(rider, drivers);

    if (matchedIndex != -1) {
        int driverStart = drivers[matchedIndex].location;

        // Find paths
        auto pathToRider = manager.findShortestPath(driverStart, rider.location);
        auto ridePath = manager.findShortestPath(rider.location, rider.destination);

        // Distance calculator lambda
        auto pathDistance = [&](const vector<int>& path) {
            double dist = 0.0;
            for (size_t i = 1; i < path.size(); ++i)
                for (const auto& nei : g.getNeighbors(path[i - 1]))
                    if (nei.first == path[i]) {
                        dist += nei.second;
                        break;
                    }
            return dist;
        };

        double pickupDist = pathDistance(pathToRider);
        double rideDist = pathDistance(ridePath);

        // Build string representations of paths for logging
        std::string pathToRiderStr, pathToDestinationStr;
        for (int n : pathToRider) pathToRiderStr += std::to_string(n) + " ";
        for (int n : ridePath) pathToDestinationStr += std::to_string(n) + " ";

        // Log to ride_log.txt (detailed log)
        Logger::logRideToFile(
            rider.id, drivers[matchedIndex].id, rider.location, rider.destination,
            pickupDist, rideDist, pathToRiderStr, pathToDestinationStr
        );

        // --- Export the full trip path for visualization ---
        // Combine full trip path: driver->rider + rider->destination
        std::vector<int> fullTripPath = pathToRider;
        if (!ridePath.empty()) {
            // Avoid duplicating pickup node
            if (!fullTripPath.empty() && fullTripPath.back() == ridePath.front())
                fullTripPath.insert(fullTripPath.end(), ridePath.begin() + 1, ridePath.end());
            else
                fullTripPath.insert(fullTripPath.end(), ridePath.begin(), ridePath.end());
        }

        // Appending to visualization_paths.csv
        std::ofstream csv_out("visualization_paths.csv", std::ios::app);
        if (csv_out) {
            int pathOrder = 1;
            for (int nId : fullTripPath) {
                auto it = nodePositions.find(nId);
                if (it != nodePositions.end()) {
                    double lat = it->second.first;
                    double lon = it->second.second;
                    csv_out << rider.id << "," << pathOrder << "," << nId << "," << lat << "," << lon << "\n";
                }
                pathOrder++;
            }
        }
        csv_out.close();

        // Update driver status to Available after ride completion
        drivers[matchedIndex].location = rider.destination;
        drivers[matchedIndex].status = DriverStatus::Available;

        // Remove the rider as ride is completed
        riders.erase(riders.begin());

        // Show concise console message
        cout << "Rider " << rider.id << " matched to Driver " << drivers[matchedIndex].id
             << ". Details saved to log and visualization CSV.\n";
    }
    else {
        Logger::logNoMatch(rider.id);
    }
    break;
}




        // 6. Show simulation summary
        case 6: {
            cout << "\nSimulation Summary:\n";
            cout << "Total Drivers: " << drivers.size() << "\n";
            cout << "Total Riders waiting: " << riders.size() << "\n";
            int available = count_if(drivers.begin(), drivers.end(), [](const Driver& d) { return d.status == DriverStatus::Available; });
            cout << "Available Drivers: " << available << "\n";
            break;
        }

        // 7. Remove a driver
        case 7: {
            if (drivers.empty()) {
                cout << "No drivers to remove.\n";
                break;
            }
            cout << "Enter driver ID to remove: ";
            int removeId;
            cin >> removeId;
            auto it = remove_if(drivers.begin(), drivers.end(), [removeId](const Driver& d) { return d.id == removeId; });
            if (it != drivers.end()) {
                drivers.erase(it, drivers.end());
                cout << "Driver removed.\n";
            } else {
                cout << "Driver ID not found.\n";
            }
            break;
        }

        // 8. Remove a rider
        case 8: {
            if (riders.empty()) {
                cout << "No riders to remove.\n";
                break;
            }
            cout << "Enter rider ID to remove: ";
            int removeId;
            cin >> removeId;
            auto it = remove_if(riders.begin(), riders.end(), [removeId](const Rider& r) { return r.id == removeId; });
            if (it != riders.end()) {
                riders.erase(it, riders.end());
                cout << "Rider removed.\n";
            } else {
                cout << "Rider ID not found.\n";
            }
            break;
        }

        // 9. View ride log (show entire file)
        case 9: {
            cout << "\n--- Ride Log File ---\n";
            Logger::printLogFile("ride_log.txt");
            cout << "--- End of Logs ---\n";
            break;
        }

        // 0. Exit
        case 0: {
            running = false;
            cout << "Exiting simulation. Goodbye!\n";
            break;
        }

        default: {
            cout << "Invalid option. Try again.\n";
            break;
        }
        } // end switch
    } // end while

    return 0;
}
