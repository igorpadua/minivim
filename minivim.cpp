#include "minivim.hpp"

Minivim::Minivim(const std::string& filename) :
    m_filename(filename)
  , m_buffer()
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

//    while (getmaxyx(stdscr, LINES, COLS)) {
//        auto ch = getch();
//        auto str = std::to_string(ch);
//        m_buffer.push_back(str);

//        for (int i = 0; i < m_buffer.size(); ++i) {
//            mvprintw(0, i, m_buffer[i].c_str());
//        }
    //    }
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

}
