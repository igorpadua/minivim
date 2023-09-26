#pragma once

#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>

class Minivim
{
public:
    Minivim(const std::string& filename = "untitled");
    ~Minivim();
    void run();

protected:
    void update();
    void statusLine() const;
    void input(const int &ch);
    void print() const;

    void up();
    void left();
    void right();
    void down();

private:
    std::string m_filename, m_status;
    std::vector<std::string> m_buffer;
    int m_cursorX, m_cursorY;
    char m_mode;

    void remove(const int &ch);
    std::string tabs(std::string &str);
    void insert(std::string str, const int &ch);
    void append(std::string &str);
};
