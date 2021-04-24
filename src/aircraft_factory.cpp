#include "aircraft_factory.hpp"


AircraftFactory::~AircraftFactory()
{
    delete airport;
}

std::unique_ptr<Aircraft> AircraftFactory::create_aircraft(const AircraftType& type)
{
    assert(airport); // make sure the airport is initialized before creating aircraft

    for(;;){
        auto flight_number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
        auto found = flight_numbers.find(flight_number);
        if(found != flight_numbers.end()) { continue; }
        flight_numbers.insert(flight_number);
        const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
        const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
        const Point3D direction = (-start).normalize();
        return std::make_unique<Aircraft>(type, flight_number, start, direction, airport->get_tower());
    }
}

std::unique_ptr<Aircraft> AircraftFactory::create_random_aircraft()
{
    return create_aircraft(*(aircraft_types[rand() % 3]));
}

void AircraftFactory::init_airport(AircraftManager& manager)
{
    airport = new Airport { one_lane_airport, Point3D { 0, 0, 0 },
                            new img::Image { one_lane_airport_sprite_path.get_full_path() },
                            manager };


    GL::move_queue.emplace(airport);
}


void AircraftFactory::display_nb_of_aircraft_from_airline(unsigned int airline_nb)
{
    assert(airline_nb < airlines.size());
    std::string airline_code = airlines[airline_nb];
    auto count = std::count_if(flight_numbers.begin(),flight_numbers.end(),[this, airline_nb](const std::string& flight_nb){
        return airlines[airline_nb][0] == flight_nb[0] && airlines[airline_nb][1] == flight_nb[1]; 
        });
    std::cout << count << " aircrafts belong to " << airlines[airline_nb]  << std::endl;
}