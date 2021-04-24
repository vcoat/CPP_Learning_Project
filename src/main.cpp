#include "tower_sim.hpp"

/*
void test_generic_points()
{
    Point2D p1 { 0.f,0.f};
    Point<int,3> p2;
    auto p3 = p1 + p2;

    p1 += p3;
}*/


int main(int argc, char** argv)
{
    TowerSimulation simulation { argc, argv };
    simulation.launch();
    //test_generic_points();
    return 0;
}