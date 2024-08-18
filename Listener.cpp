#include <windows.h>
#include "Listener.h"
#include <iostream>
#include <thread>

#include "IOUtil.h"
#include "TodoList.h"
#include "OutputUtil.h"
#include "StringColor.h"

const int Listener::up = 38;
const int Listener::down = 40;
const int Listener::left = 37;
const int Listener::right = 39;
const int Listener::esc = 27;
const int Listener::space = 32;
const int Listener::a = 65;
const int Listener::d = 68;
const int Listener::enter = 13;

int Listener::stage = 0;
unsigned long long Listener::last = 0;
int Listener::lastItem = -1;

void Listener::mouseClickListener(COORD pos) {
    unsigned long long d = GetTickCount64();
    if (d - last < 100) return;
    last = d;
    if (stage == 1) {
        if ((pos.X >= IOUtil::winsize.X * 4 / 5 && pos.X < IOUtil::winsize.X * 4 / 5 + 4) &&  pos.Y >= 7 && pos.Y % 2 == 1 && (pos.Y - 7) / 2 < up && (pos.Y - 7) / 2 + TodoList::start < TodoList::list.size()) {
            if (lastItem == (pos.Y - 7) / 2 + TodoList::start) {
                if (TodoList::list[(pos.Y - 7) / 2 + TodoList::start].complete) TodoList::nowCompleteCount--;
                for(int i = (pos.Y - 7) / 2 + TodoList::start;i < TodoList::list.size() - 1;++i)
                    TodoList::list[i] = TodoList::list[i + 1];
                if (TodoList::list.size() <= TodoList::start + TodoList::up)
                    IOUtil::write(OutputUtil::clearFull,IOUtil::winsize.X / 5,(TodoList::list.size() - TodoList::start - 1) * 2 + 7);
                TodoList::list.pop_back();
                IOUtil::writeToFile();
                IOUtil::write(OutputUtil::clearCount,IOUtil::winsize.X / 5 + 6,3);
                OutputUtil::outputList();
                lastItem = -1;
            } else {
                if (lastItem != -1) TodoList::list[lastItem].del = false;
                lastItem = (pos.Y - 7) / 2 + TodoList::start;
                TodoList::list[lastItem].del = true;
                OutputUtil::outputList();
            }
        } else if (lastItem != -1 && lastItem >= TodoList::start && lastItem < TodoList::start + TodoList::up) {
            if (lastItem != -1) TodoList::list[lastItem].del = false;
            lastItem = -1;
            OutputUtil::outputList();
        }

        if (pos.Y == 3 && (pos.X >= IOUtil::winsize.X * 4 / 5 && pos.X < IOUtil::winsize.X * 4 / 5 + 4)) {
            stage = 2;
            //TodoList::list.emplace_back("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz","fdas",10800,false);
            OutputUtil::outputNewPage();
            stage = 1;
            IOUtil::writeToFile();
            TodoList::sortList();
            OutputUtil::outputMainPage();
        } else if ((pos.X >= IOUtil::winsize.X * 4 / 5 - 5 && pos.X < IOUtil::winsize.X * 4 / 5 - 1) &&  pos.Y >= 7 && pos.Y % 2 == 1 && (pos.Y - 7) / 2 < up && (pos.Y - 7) / 2 + TodoList::start < TodoList::list.size()) {
            stage = 3;
            TodoList::now = TodoList::start + (pos.Y - 7) / 2;
            std::thread* th = new std::thread(OutputUtil::outputAttation,TodoList::start + (pos.Y - 7) / 2,TodoList::id);
            th->detach();
        } else if (pos.Y == 5 && (pos.X >= IOUtil::winsize.X / 5 && pos.X < IOUtil::winsize.X / 5 + 4)) {
            if (TodoList::sortType == 0) TodoList::sortType = 1;
            else TodoList::sortType = 0;
            TodoList::sortList();
            TodoList::start = 0;
            OutputUtil::outputList();
        } else if (pos.Y == 5 && (pos.X >= IOUtil::winsize.X * 4 / 5 - 30 && pos.X < IOUtil::winsize.X * 4 / 5 - 26)) {
            if (TodoList::sortType == 2) TodoList::sortType = 3;
            else TodoList::sortType = 2;
            TodoList::sortList();
            TodoList::start = 0;
            OutputUtil::outputList();
        } else if (pos.Y == 5 && (pos.X >= IOUtil::winsize.X * 4 / 5 - 12 && pos.X < IOUtil::winsize.X * 4 / 5 - 8)) {
            if (TodoList::sortType == 4) TodoList::sortType = 5;
            else TodoList::sortType = 4;
            TodoList::sortList();
            TodoList::start = 0;
            OutputUtil::outputList();
        } else if (pos.Y == 5 && (pos.X >= IOUtil::winsize.X * 4 / 5 - 21 && pos.X < IOUtil::winsize.X * 4 / 5 - 17)) {
            if (TodoList::sortType == 6) TodoList::sortType = 7;
            else TodoList::sortType = 6;
            TodoList::sortList();
            TodoList::start = 0;
            OutputUtil::outputList();
        } else if (pos.Y == IOUtil::winsize.Y / 2 + 3 && pos.X >= 5 && pos.X < 13) {
            TodoList::nowItem = 1;
            stage = 4;
            OutputUtil::outputOperatorPage();
        }
    } else if (stage == 4) {
        if (pos.Y == IOUtil::winsize.Y / 2 - 3 && pos.X >= 5 && pos.X < 13) {
            TodoList::nowItem = 0;
            stage = 1;
            OutputUtil::outputMainPage();
        }
    }
}

void Listener::mouseMoveListener(COORD pos) {
}

void Listener::mouseWheelListener(DWORD state) {
    if (state == 0xFF800000) {
        if (TodoList::start + TodoList::up < TodoList::list.size()) {
            TodoList::start++;
            OutputUtil::outputList();
        }
    } else if (state == 0x800000){
        if (TodoList::start > 0) {
            TodoList::start--;
            OutputUtil::outputList();
        }
    }
    // 4286578688 向下
    // 8388608 向上
}

void Listener::keyListener(KEY_EVENT_RECORD &key) {
    switch(key.wVirtualKeyCode) {
        case esc:
            if (stage == 2) {
                stage = 1;
                OutputUtil::outputMainPage();
            } else if (stage == 3) {
                if (TodoList::list[TodoList::now].complete) {
                    stage = 1;
                    OutputUtil::outputMainPage();
                    TodoList::id++;
                } else {
                    IOUtil::writeWithColor("当前正在专注模式，不可退出！",IOUtil::winsize.X / 2 - 14,IOUtil::winsize.Y - 3,StringColor::red,"");
                }
            }
            break;
        default:
            break;
    }
}
