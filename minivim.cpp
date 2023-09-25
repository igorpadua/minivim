#include "minivim.hpp"

Minivim::Minivim(const std::string& filename) :
    m_filename(filename)
  , m_buffer()
  , m_cursorX(0) , m_cursorY(0)
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

void Minivim::statusLine()
{
    attron(A_REVERSE);
    mvprintw(LINES - 1, 0, "%s", m_status.c_str());
    attroff(A_REVERSE);
}

void Minivim::input(int ch)
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
        return;
    case 'i':
        switch (ch) {
        case 27:
            m_mode = 'n';
            break;
        default:
            auto str = std::string(1, ch);
            m_buffer.push_back(str);
        }
    }

    for (int i = 0; i < m_buffer.size(); ++i) {
        mvprintw(0, i, "%s", m_buffer[i].c_str());
    }
}
