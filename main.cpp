#include "minivim.hpp"

int main(int argc, char** argv)
{
    std::shared_ptr<Minivim> minivim;

    if (argc > 1) {
        minivim = std::make_shared<Minivim>(argv[1]);
    } else {
        minivim = std::make_shared<Minivim>();
    }

    minivim->run();

    return 0;
}
