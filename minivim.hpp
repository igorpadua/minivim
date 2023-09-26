#pragma once

#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

class Minivim
{
public:
    Minivim(const std::string& filename = "untitled");
    ~Minivim();
    void run();

protected:
    void updateStatus();
    void statusLine() const;
    void input(const int &ch);
    void print() const;

    void up();
    void left();
    void right();
    void down();

    void open();
    void save();

private:
    std::string m_filename, m_status, m_section;
    std::vector<std::string> m_buffer;
    int m_cursorX, m_cursorY;
    char m_mode;

    void remove(const int &ch);
    std::string tabs(std::string &str);
    void insert(std::string str, const int &ch);
    void append(std::string &str);

    void normalMode(const int &ch);
};
