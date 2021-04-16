#include "tower_sim.hpp"

int main(int argc, char** argv)
{

    MediaPath::initialize(argv[0]);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    GL::init_gl(argc, argv, "Airport Tower Simulation");

    TowerSimulation simulation { argc, argv };
    simulation.launch();

    test_generic_points();

    return 0;
}