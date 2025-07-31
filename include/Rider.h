#pragma once

class Rider {
public:
    int id;
    int location;      // Node ID where the rider is waiting
    int destination;   // Node ID for the destination

    Rider(int rider_id, int start_location, int destination_location);
};
