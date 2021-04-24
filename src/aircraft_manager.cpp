#include "aircraft_manager.hpp"


void AircraftManager::add(std::unique_ptr<Aircraft> aircraft)
{
    aircrafts.emplace_back(std::move(aircraft));
}

unsigned int AircraftManager::get_required_fuel()
{   
    unsigned int sum = 0u;
    std::for_each(aircrafts.begin(),aircrafts.end(),[&sum](const std::unique_ptr<Aircraft>& a) mutable {
        if(a->is_low_on_fuel() && !a->service_done())
        {
            sum += (3000u - a->get_fuel());
        }
    });
    return sum;
}

bool AircraftManager::update()
{   
    std::sort(aircrafts.begin(),aircrafts.end(),[](const std::unique_ptr<Aircraft>& a,const std::unique_ptr<Aircraft>& b){
        if(a->has_terminal() && !b->has_terminal())
        {
            return true;    
        }
        else if (!a->has_terminal() && b->has_terminal())
        {
            return false;
        }
        else
        {
            if(a->get_fuel() < b->get_fuel())
            {
                return true;
            }
            return false;
        }
    });
    
    aircrafts.erase(std::remove_if(
        aircrafts.begin(),
        aircrafts.end(),
        [this](const std::unique_ptr<Aircraft>& aircraft){
            
            try{
                auto result = aircraft->update();
                return !result;
            }catch(const std::runtime_error& error){
                std::cerr << error.what() << std::endl;
                crash_counter += 1;
                return true;
            }
        }),
        aircrafts.end());

    return true;
}

