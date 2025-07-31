#pragma once

enum class DriverStatus { Available, EnRoute, Busy };

class Driver {
public:
    int id;
    int location;           // Node ID where the driver is currently
    DriverStatus status;

    Driver(int driver_id, int start_location);
};
