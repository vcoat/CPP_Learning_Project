#pragma once

#include "aircraft_manager.hpp"
#include "aircraft_factory.hpp"


class Aircraft;
struct AircraftType;
class Airport;

class TowerSimulation
{
private:
    bool help        = false;
    
    AircraftManager manager;
    AircraftFactory factory;

    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    

    void create_keystrokes();
    void display_help() const;

    

public:
    TowerSimulation(int argc, char** argv);

    void launch();
};
