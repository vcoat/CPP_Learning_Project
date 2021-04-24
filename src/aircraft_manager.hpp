#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"

#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>

class AircraftManager : public GL::DynamicObject
{
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;
    unsigned int crash_counter = 0;

public:
    void add(std::unique_ptr<Aircraft> aircraft);
    unsigned int get_required_fuel();
    bool update() override;
    void display_crash_counter() 
    {   
        if(crash_counter == 0) { std::cout << crash_counter << " plane have crashed so far :)" << std::endl; }
        else { std::cout << crash_counter << " planes have crashed so far :(" << std::endl; }
    }
};
