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
    switch (ch) {
    case KEY_UP:
        up();
        return;
    case KEY_LEFT:
        left();
        return;
    case KEY_RIGHT:
        right();
        return;
    case KEY_DOWN:
        down();
        return;
    }

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
        case KEY_BACKSPACE:
            if (m_cursorX == 0 && m_cursorY > 0) {
                m_cursorX = m_buffer[m_cursorY - 1].length();
                m_buffer[m_cursorY - 1] += m_buffer[m_cursorY];
                remove(m_cursorY);
            } else if (m_cursorX > 0) {
                m_buffer[m_cursorY].erase(m_cursorX, 1);
                --m_cursorX;
            }
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

void Minivim::up()
{
    if (m_cursorY > 0) {
        --m_cursorY;
    }

    if (m_cursorX >= m_buffer[m_cursorY].length()) {
        m_cursorX = m_buffer[m_cursorY].length();
    }

    move(m_cursorY, m_cursorX);
}

void Minivim::left()
{
    if (m_cursorX > 0) {
        --m_cursorX;
        move(m_cursorY, m_cursorX);
    }
}

void Minivim::right()
{
    if (m_cursorX <= COLS && m_cursorX <= m_buffer[m_cursorY].length() - 1) {
        ++m_cursorX;
        move(m_cursorY, m_cursorX);
    }
}

void Minivim::down()
{
    if (m_cursorY < LINES && m_cursorY < m_buffer.size() - 1) {
        ++m_cursorY;
    }

    if (m_cursorX >= m_buffer[m_cursorY].length()) {
        m_cursorX = m_buffer[m_cursorY].length();
    }

    move(m_cursorY, m_cursorX);
}

void Minivim::remove(const int &ch)
{
    m_buffer.erase(m_buffer.begin() + ch);
}

std::string Minivim::tabs(std::string &str)
{
    auto tab = str.find('\t');
    return tab == str.npos ? str : tabs(str.replace(tab, 1, "  "));
}

void Minivim::insert(std::string str, const int &ch)
{
    str = tabs(str);
    m_buffer.insert(m_buffer.begin() + ch, str);
}

void Minivim::append(std::string &str)
{
    str = tabs(str);
    m_buffer.push_back(str);
}

