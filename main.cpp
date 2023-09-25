#include "minivim.hpp"

int main(int argc, char** argv)
{
    std::unique_ptr<Minivim> minivim;

    if (argc > 1) {
        minivim = std::make_unique<Minivim>(argv[1]);
    } else {
        minivim = std::make_unique<Minivim>();
    }

    minivim->run();

    return 0;
}
