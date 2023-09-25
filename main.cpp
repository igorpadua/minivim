#include "minivim.hpp"

int main()
{
    auto minivim = std::make_shared<Minivim>();
    minivim->run();
    return 0;
}
