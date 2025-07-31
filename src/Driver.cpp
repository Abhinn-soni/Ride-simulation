#include "Driver.h"

Driver::Driver(int driver_id, int start_location)
    : id(driver_id), location(start_location), status(DriverStatus::Available) {}

