#include "minivim.hpp"

Minivim::Minivim(const std::string& filename) :
    m_filename(filename)
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
}

Minivim::~Minivim()
{
    refresh();
    endwin();
}

void Minivim::run()
{
    printw("%s", m_filename.c_str());
    getch();
}
