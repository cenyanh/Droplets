#ifndef OIUTIL_H
#define OIUTIL_H
#include <windows.h>
#include <iostream>

class IOUtil {
public:
    static HANDLE hin;
    static HANDLE hout;
    static POINT pos;
    static HWND hwin;
    static COORD winsize;
    static COORD startpos;
    static INPUT_RECORD input_record;
    static DWORD input_res;
    static DWORD buflen;

    static void banMode();

    static void move(SHORT x, SHORT y);

    static POINT getCursorPosition();

    static void resize();

    static void inputListener();

    static void write(std::string content, SHORT x, SHORT y);

    static void writeWithColor(std::string content, SHORT x, SHORT y,std::string color, std::string background);

    static void writeToFile();
};

#endif //OIUTIL_H
