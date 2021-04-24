#pragma once


#include "aircraft.hpp"
#include "airport.hpp"
#include "aircraft_manager.hpp"

#include <memory>
#include <vector>
#include <algorithm>


class AircraftFactory{

    public :
    
        [[nodiscard]] std::unique_ptr<Aircraft> create_aircraft(const AircraftType& type);
        [[nodiscard]] std::unique_ptr<Aircraft> create_random_aircraft();
        void init_airport(AircraftManager& manager);
        void display_nb_of_aircraft_from_airline(unsigned int airline_nb);
        ~AircraftFactory();

    private:

        Airport* airport = nullptr;
        std::unordered_set<std::string> flight_numbers;
        const std::vector<std::string> airlines = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };
        

};

