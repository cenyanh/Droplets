#include <iostream>
#include <fstream>
#include <windows.h>
#include <thread>
#include "Listener.h"
#include "OutputUtil.h"
#include "IOUtil.h"
#include "TodoList.h"
using namespace std;
int main() {
    SetConsoleTitle("Droplets");

    IOUtil::banMode();

    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(IOUtil::hout,&cci);
    cci.bVisible = false;
    SetConsoleCursorInfo(IOUtil::hout,&cci);

    IOUtil::resize();

    thread* input_read = new thread(IOUtil::inputListener);
    input_read->detach();

    thread* outdate = new thread([&]() {
        while(true) {
            time_t now = time(nullptr);
            tm* tt = localtime(&now);
            for(TodoItem &ti:TodoList::list) {
                if (tt->tm_year + 1900 > ti.year || (tt->tm_year + 1900 == ti.year && (tt->tm_mon + 1 > ti.month || (tt->tm_mon + 1 == ti.month && tt->tm_mday > ti.day))) && ti.log == 0) {
                    ti.log = 1;
                    TodoList::dateCount++;
                }
            }
            Sleep(1000);
        }
    });
    outdate->detach();

    thread* read = new thread([&]() {
        ifstream fp;
        fp.open("items.txt",ios_base::in);

        if (fp.is_open()) {
            int n;
            fp >> n >> TodoList::sumCount >> TodoList::sumTime >> TodoList::completeCount >> TodoList::dateCount >> TodoList::longTime >> TodoList::nowCompleteCount;
            TodoItem ti;
            for(int i = 0;i < n;++i) {
                fp >> ti.title >> ti.duration >> ti.date >> ti.year >> ti.month >> ti.day >> ti.complete >> ti.log;
                fp.ignore();
                getline(fp,ti.description);
                TodoList::list.push_back(ti);
            }
            TodoList::sortList();
        } else {
            ofstream ofs;
            ofs.open("items.txt",ios_base::out);
            ofs << "0 0 0 0 0 0";
            ofs.close();
        }

        fp.close();
    });
    read->detach();

    OutputUtil::init(IOUtil::winsize.X,IOUtil::winsize.Y);
    Listener::stage = 1;

    IOUtil::hwin = FindWindow(nullptr,"Droplets");

    while(true) Sleep(0x3f3f3f3f);
}