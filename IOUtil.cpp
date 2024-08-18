#include "IOUtil.h"
#include "Listener.h"
#include "StringColor.h"
#include <fstream>
#include <windows.h>
#include <iostream>

#include "TodoList.h"


HANDLE IOUtil::hin = GetStdHandle(STD_INPUT_HANDLE);
HANDLE IOUtil::hout = GetStdHandle(STD_OUTPUT_HANDLE);
POINT IOUtil::pos;
HWND IOUtil::hwin;
COORD IOUtil::winsize;
COORD IOUtil::startpos = {0, 0};
INPUT_RECORD IOUtil::input_record;
DWORD IOUtil::input_res;
DWORD IOUtil::buflen;

void IOUtil::banMode() {
    DWORD mode;
    GetConsoleMode(hin, &mode);
    mode = (mode | ENABLE_MOUSE_INPUT) & (~ENABLE_QUICK_EDIT_MODE);
    SetConsoleMode(hin, mode);

    GetConsoleMode(hout, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hout, mode);
}

void IOUtil::move(const SHORT x, const SHORT y) {
    SetConsoleCursorPosition(hout, {x, y});
}

POINT IOUtil::getCursorPosition() {
    GetCursorPos(&pos);
    ScreenToClient(hwin, &pos);
    return pos;
}

void IOUtil::resize() {
    winsize.X = 120;
    winsize.Y = 30;
    buflen = winsize.X * winsize.Y;
    _SMALL_RECT sr{0,0,(short)(winsize.X - 1),(short)(winsize.Y - 1)};
    SetConsoleScreenBufferSize(hout,winsize);
    SetConsoleWindowInfo(hout,true,&sr);
}

void IOUtil::inputListener() {
    while (ReadConsoleInputW(hin, &input_record, 1, &input_res)) {
        switch (input_record.EventType) {
            case MOUSE_EVENT:
                if (input_record.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
                    Listener::mouseClickListener(input_record.Event.MouseEvent.dwMousePosition);
                else if (input_record.Event.MouseEvent.dwEventFlags == MOUSE_MOVED)
                    Listener::mouseMoveListener(input_record.Event.MouseEvent.dwMousePosition);
                else if (input_record.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED)
                    Listener::mouseWheelListener(input_record.Event.MouseEvent.dwButtonState);
                break;
            case KEY_EVENT:
                if (input_record.Event.KeyEvent.bKeyDown)
                    Listener::keyListener(input_record.Event.KeyEvent);
                break;
            case WINDOW_BUFFER_SIZE_EVENT:
                break;
            default:
                break;
        }
    }
}

void IOUtil::write(std::string content, SHORT x,SHORT y) {
    move(x,y);
    WriteFile(hout,content.c_str(),content.size(),nullptr,nullptr);
}

void IOUtil::writeWithColor(std::string content, SHORT x, SHORT y, std::string color, std::string background) {
    move(x,y);
    std::cout << background << color;
    WriteFile(hout,content.c_str(),content.size(),nullptr,nullptr);
    std::cout << StringColor::clear;
}

void IOUtil::writeToFile() {
    std::ofstream ofs;
    ofs.open("items.txt",std::ios_base::out);
    ofs << TodoList::list.size() << " " << TodoList::sumCount << " " << TodoList::sumTime << " " << TodoList::completeCount << " " << TodoList::dateCount << " " << TodoList::longTime << " " << TodoList::nowCompleteCount << "\n";
    for(TodoItem &item:TodoList::list) {
        ofs << item.title << " " << item.duration << " " << item.date << " " << item.year << " " << item.month << " " << item.day << " " << item.complete << " " << item.log << "\n";
        ofs << item.description << "\n";
    }
    ofs.close();
}
