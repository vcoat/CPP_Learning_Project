#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std::string_literals;


TowerSimulation::TowerSimulation(int argc, char** argv) :
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) }
{
    MediaPath::initialize(argv[0]);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    GL::init_gl(argc, argv, "Airport Tower Simulation");

    create_keystrokes();

    GL::move_queue.emplace(&manager);
}





void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { manager.add(factory.create_random_aircraft()); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    GL::keystrokes.emplace('j', []() { (GL::ticks_per_sec == 1 ? GL::ticks_per_sec = 1 : GL::ticks_per_sec -=1); });
    GL::keystrokes.emplace('k', []() { (GL::ticks_per_sec == MAX_TICKS_PER_SEC ? GL::ticks_per_sec = MAX_TICKS_PER_SEC : GL::ticks_per_sec +=1); });
    GL::keystrokes.emplace('p', []() { GL::is_paused = !GL::is_paused; });
    GL::keystrokes.emplace('m', [this]() { manager.display_crash_counter(); });
    GL::keystrokes.emplace('0', [this]() { factory.display_nb_of_aircraft_from_airline(0); });
    GL::keystrokes.emplace('1', [this]() { factory.display_nb_of_aircraft_from_airline(1); });
    GL::keystrokes.emplace('2', [this]() { factory.display_nb_of_aircraft_from_airline(2); });
    GL::keystrokes.emplace('3', [this]() { factory.display_nb_of_aircraft_from_airline(3); });
    GL::keystrokes.emplace('4', [this]() { factory.display_nb_of_aircraft_from_airline(4); });
    GL::keystrokes.emplace('5', [this]() { factory.display_nb_of_aircraft_from_airline(5); });
    GL::keystrokes.emplace('6', [this]() { factory.display_nb_of_aircraft_from_airline(6); });
    GL::keystrokes.emplace('7', [this]() { factory.display_nb_of_aircraft_from_airline(7); });

}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keystrokes have meaning:" << std::endl;

    for (const auto& [key,_] : GL::keystrokes)
    {
        std::cout << key << ' ';
    }

    std::cout << std::endl;
}



void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    factory.init_airport(manager);
    init_aircraft_types();

    GL::loop();
}
