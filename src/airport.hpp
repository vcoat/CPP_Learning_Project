#pragma once

#include "GL/displayable.hpp"
#include "GL/dynamic_object.hpp"
#include "GL/texture.hpp"
#include "airport_type.hpp"
#include "geometry.hpp"
#include "img/image.hpp"
#include "runway.hpp"
#include "terminal.hpp"
#include "tower.hpp"
#include "aircraft_manager.hpp"

#include <vector>

class Airport : public GL::Displayable, public GL::DynamicObject
{
private:
    unsigned int fuel_stock = 0u;
    unsigned int ordered_fuel = 0u;
    unsigned int next_refill_time = 0u;

    
    const AirportType& type;
    const Point3D pos;
    const GL::Texture2D texture;
    std::vector<Terminal> terminals;
    Tower tower;
    AircraftManager& manager;

    // reserve a terminal
    // if a terminal is free, return
    // 1. a sequence of waypoints reaching the terminal from the runway-end and
    // 2. the number of the terminal (used for liberating the terminal later)
    // otherwise, return an empty waypoint-vector and any number
    std::pair<WaypointQueue, size_t> reserve_terminal(Aircraft& aircraft)
    {
        const auto it =
            std::find_if(terminals.begin(), terminals.end(), [](const Terminal& t) { return !t.in_use(); });

        if (it != terminals.end())
        {
            it->assign_craft(aircraft);
            const auto term_idx = std::distance(terminals.begin(), it);
            return { type.air_to_terminal(pos, 0, term_idx), term_idx };
        }
        else
        {
            return { {}, 0u };
        }
    }

    WaypointQueue start_path(const size_t terminal_number)
    {
        return type.terminal_to_air(pos, 0, terminal_number);
    }

    Terminal& get_terminal(const size_t terminal_num) { return terminals.at(terminal_num); }

public:
    Airport(const AirportType& type_, const Point3D& pos_, const img::Image* image, AircraftManager& manager_,const float z_ = 1.0f) :
        GL::Displayable { z_ },
        type { type_ },
        pos { pos_ },
        texture { image },
        terminals { type.create_terminals()},
        tower { *this },
        manager {manager_}
    {
        std::for_each(terminals.begin(),terminals.end(),[this](Terminal& term){term.initialize_fuel_stock(fuel_stock);});
    }

    Tower& get_tower() { return tower; }

    unsigned int& get_fuel_stock() { return fuel_stock;}

    void display() const override { texture.draw(project_2D(pos), { 2.0f, 2.0f }); }

    bool update() override
    {   
        if(next_refill_time == 0u)
        {
            fuel_stock += ordered_fuel;
            std::cout << "Received " << ordered_fuel << " liters of fuel" << std::endl;
            std::cout << fuel_stock << " liters of fuel currently in stock." << std::endl;
            auto required_fuel = manager.get_required_fuel();
            if(required_fuel > fuel_stock) // order needed
            {
                ordered_fuel = std::min(required_fuel - fuel_stock,5000u);  
            }
            else
            {
                ordered_fuel = 0u;
            }
            std::cout << "Ordered " << ordered_fuel << " liters of fuel" << std::endl;
            next_refill_time = 100u;
            
        }else{
            next_refill_time -= 1u;
        }


        for (auto& t : terminals)
        {
            t.update();
        }
        return true;
    }

    friend class Tower;
};
