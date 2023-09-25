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
    void statusLine();

private:
    std::string m_filename, m_status;
    std::vector<std::string> m_buffer;
    int m_cursorX, m_cursorY;
    char m_mode;
};
