#include "minivim.hpp"
#include <iostream>

Minivim::Minivim(const std::string& filename) :
    m_filename(filename)
  , m_cursorX(0)
  , m_cursorY(0)
  , m_mode('n')
  , m_status("NORMAL")
  , m_section()
{
    open();
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    use_default_colors();
}

Minivim::~Minivim()
{
    refresh();
    endwin();
}

void Minivim::run()
{
    while (m_mode != 'q') {
        updateStatus();
        statusLine();
        print();
        auto c = getch();
        input(c);
    }
}

void Minivim::updateStatus()
{
    switch (m_mode) {
    case 'n':
        m_status = "NORMAL";
        break;
    case 'i':
        m_status = "INSERT";
        break;
    case 'q':
        exit(0);
        break;
    }

    m_section = " COLS: " + std::to_string(m_cursorX) + "| ROWS: " + std::to_string(m_cursorY) + " | FILE: " + m_filename;
    m_status.insert(0, " ");
}

void Minivim::statusLine() const
{
    start_color();

    if (m_mode == 'n') {
        init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    } else {
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
    }

    attron(A_REVERSE);
    attron(A_BOLD);
    attron(COLOR_PAIR(1));
    for (int i = 0; i < COLS; ++i) {
        mvprintw(LINES - 1, i, " ");
    }
    mvprintw(LINES - 1, 0, "%s", m_status.c_str());
    mvprintw(LINES - 1, COLS - m_section.length(), "%s", m_section.c_str());
    attroff(COLOR_PAIR(1));
    attroff(A_BOLD);
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
        normalMode(ch);
        break;
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
                up();
            } else if (m_cursorX > 0) {
                m_buffer[m_cursorY].erase(m_cursorX, 1);
                --m_cursorX;
            }
            break;
        case KEY_DC:
            if (m_cursorX == m_buffer[m_cursorY].length() && m_cursorY != m_buffer.size() - 1) {
                m_buffer[m_cursorY] += m_buffer[m_cursorY + 1];
            } else {
                m_buffer[m_cursorY].erase(m_cursorX, 1);
            }
            break;
        case 10:
        case KEY_ENTER:
            if (m_cursorX < m_buffer[m_cursorY].length()) {
                insert(m_buffer[m_cursorY].substr(m_cursorX, m_buffer[m_cursorY].length() - m_cursorX), m_cursorY + 1);
                m_buffer[m_cursorY].erase(m_cursorX, m_buffer[m_cursorY].length() - m_cursorX);
            } else {
                insert("", m_cursorY + 1);
                m_cursorX = 0;
                down();
            }
            break;
        case KEY_BTAB:
        case KEY_CTAB:
        case KEY_STAB:
        case KEY_CATAB:
        case 9:
            m_buffer[m_cursorY].insert(m_cursorX, 2, ' ');
            m_cursorX += 2;
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

void Minivim::open()
{
    if (std::filesystem::exists(m_filename)) {
        std::ifstream ifile(m_filename);
        if (ifile.is_open()) {
            while (!ifile.eof()) {
                std::string buffer;
                std::getline(ifile, buffer);
                append(buffer);
            }
        } else {
            throw std::runtime_error("Cannot open file. Permission denied.");
        }
    } else {
        std::string str{};
        append(str);
    }
}

void Minivim::save()
{
    std::ofstream ofile(m_filename);

    if (ofile.is_open()) {
        for (int i = 0; i < m_buffer.size(); ++i) {
            ofile << m_buffer[i] << '\n';
        }
        ofile.close();
    } else {
        refresh();
        endwin();
        throw std::runtime_error("Cannot save file. Permission denied.");
    }
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

void Minivim::normalMode(const int &ch)
{
    switch (ch) {
    case 'q':
        m_mode = 'q';
        break;
    case 'i':
        m_mode = 'i';
        break;
    case 'w':
        m_mode = 'w';
        save();
        exit(0);
        break;
    }
}

