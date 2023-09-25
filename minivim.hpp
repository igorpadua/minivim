#pragma once

#include <memory>
#include <ncurses/ncurses.h>
#include <string>

class Minivim
{
public:
    Minivim(const std::string& filename = "untitled");
    ~Minivim();
    void run();

private:
    std::string m_filename;
};
