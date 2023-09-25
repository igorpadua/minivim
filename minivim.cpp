#include "minivim.hpp"
#include <iostream>

Minivim::Minivim(const std::string& filename) :
    m_filename(filename)
  , m_buffer(1, "")
  , m_cursorX(0)
  , m_cursorY(0)
  , m_mode('n')
  , m_status("NORMAL")
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
    while (m_mode != 'q') {
        update();
        statusLine();
        print();
        auto c = getch();
        input(c);
    }
}

void Minivim::update()
{
    switch (m_mode) {
    case 27:
    case 'n':
        m_status = "NORMAL";
        break;
    case 'i':
        m_status = "INSERT";
        break;
    case 'q':
        break;
    default:
        break;
    }
}

void Minivim::statusLine() const
{
    attron(A_REVERSE);
    mvprintw(LINES - 1, 0, "%s", m_status.c_str());
    attroff(A_REVERSE);
}

void Minivim::input(const int& ch)
{
    switch (m_mode) {
    case 27:
    case 'n':
        switch (ch) {
        case 27:
        case 'n':
            m_mode = 'q';
            break;
        case 'i':
            m_mode = 'i';
            break;
        case 'w':
            m_mode = 'w';
            break;
        }
    case 'i':
        switch (ch) {
        case 27:
            m_mode = 'n';
            break;
        default:
            m_buffer[m_cursorY].insert(m_cursorX, 1, ch);
            ++m_cursorX;
        }
    }
}

void Minivim::print() const
{
    for (int i = 0; i < LINES - 1; ++i) {
        i >= m_buffer.size() ? move(i, 0) : mvprintw(i, 0, "%s", m_buffer[i].c_str());
        clrtoeol();
    }
    move(m_cursorY, m_cursorX);
}

void Minivim::remove(const int &ch)
{

}

std::string Minivim::tabs(std::string &str)
{

}

void Minivim::insert(std::string str, const int &ch)
{

}

void Minivim::append(std::string &str)
{

}

