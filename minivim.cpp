#include "minivim.hpp"

Minivim::Minivim()
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
    printw("Funcionado!");
    getch();
}
