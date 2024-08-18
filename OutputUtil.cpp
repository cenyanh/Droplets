#include <regex>
#include "OutputUtil.h"
#include "IOUtil.h"
#include "Listener.h"
#include "StringColor.h"
#include "TodoList.h"
#include <windows.h>

std::string OutputUtil::title[6] = {
    "██████╗  ██████╗   ██████╗  ██████╗  ██╗      ███████╗ ████████╗ ███████╗",
    "██╔══██╗ ██╔══██╗ ██╔═══██╗ ██╔══██╗ ██║      ██╔════╝ ╚══██╔══╝ ██╔════╝",
    "██║  ██║ ██████╔╝ ██║   ██║ ██████╔╝ ██║      █████╗      ██║    ███████╗",
    "██║  ██║ ██╔══██╗ ██║   ██║ ██╔═══╝  ██║      ██╔══╝      ██║    ╚════██║",
    "██████╔╝ ██║  ██║ ╚██████╔╝ ██║      ███████╗ ███████╗    ██║    ███████║",
    "╚═════╝  ╚═╝  ╚═╝  ╚═════╝  ╚═╝      ╚══════╝ ╚══════╝    ╚═╝    ╚══════╝"
};
std::string OutputUtil::clearLine;
std::string OutputUtil::clearFull;
std::string OutputUtil::clearCount = "          ";
void OutputUtil::init(SHORT X, SHORT Y) {
    clear();
    outputMainTitle((X - 73) / 2, (Y - 9) / 2);
    outputLoadBar((Y + 9) / 2 + 2);
    TodoList::up = (IOUtil::winsize.Y - 7) / 2;

    for(int i = IOUtil::winsize.X / 5;i <= IOUtil::winsize.X * 4 / 5 - 10;++i) {
        clearLine.push_back(' ');
    }

    for(int i = IOUtil::winsize.X / 5;i < IOUtil::winsize.X;++i) {
        clearFull.push_back(' ');
    }

    outputMainPage();
}

void OutputUtil::outputMainTitle(const SHORT x, const SHORT y) {
    for (int i = 0; i < 6; ++i) IOUtil::writeWithColor(title[i],x,y + i,StringColor::skyblue,"");
}

void OutputUtil::outputLoadBar(SHORT y) {
    int center = IOUtil::winsize.X / 2,l,r = center + 1;
    std::cout << StringColor::red;
    Sleep(500);
    if ((center & 1)) {
        l = center - 1;
        IOUtil::write("#",center,y);
    } else {
        l = center - 2;
        IOUtil::write("##",center - 1,y);
    }
    for(int i = 0;i < l;++i) {
        Sleep(25);
        IOUtil::write("#",l - i,y);
        IOUtil::write("#",r + i,y);
    }
    std::cout << StringColor::clear;
    Sleep(1000);
}

void OutputUtil::outputMainPage() {
    clear();
    TodoList::start = 0;
    outputOperatorItem();
    outputList();
}

void OutputUtil::outputOperatorItem() {
    if (TodoList::nowItem == 0) {
        IOUtil::writeWithColor("任务清单",5,IOUtil::winsize.Y / 2 - 3,StringColor::green,"");
        IOUtil::writeWithColor("信息统计",5,IOUtil::winsize.Y / 2 + 3,StringColor::red,"");
    } else {
        IOUtil::writeWithColor("任务清单",5,IOUtil::winsize.Y / 2 - 3,StringColor::red,"");
        IOUtil::writeWithColor("信息统计",5,IOUtil::winsize.Y / 2 + 3,StringColor::green,"");
    }
}

void OutputUtil::outputOperatorPage() {
    clear();
    outputOperatorItem();
    outputInformation();
}

void OutputUtil::outputInformation() {
    IOUtil::writeWithColor("总创建待办事项：",IOUtil::winsize.X / 5,IOUtil::winsize.Y / 2 - 10,StringColor::green,"");
    IOUtil::writeWithColor(TodoList::parseToString(TodoList::sumCount),IOUtil::winsize.X / 5 + 16,IOUtil::winsize.Y / 2 - 10,StringColor::green,"");

    IOUtil::writeWithColor("总专注时长：",IOUtil::winsize.X / 5,IOUtil::winsize.Y / 2 - 6,StringColor::green,"");
    IOUtil::writeWithColor(TodoList::parseToString(TodoList::sumTime / 86400) + "d " + TodoList::parseToTime(TodoList::sumTime % 86400),IOUtil::winsize.X / 5 + 12,IOUtil::winsize.Y / 2 - 6,StringColor::green,"");

    IOUtil::writeWithColor("总专注完成次数：",IOUtil::winsize.X / 5,IOUtil::winsize.Y / 2 - 2,StringColor::green,"");
    IOUtil::writeWithColor(TodoList::parseToString(TodoList::completeCount),IOUtil::winsize.X / 5 + 16,IOUtil::winsize.Y / 2 - 2,StringColor::green,"");

    IOUtil::writeWithColor("总逾期待办事项：",IOUtil::winsize.X / 5,IOUtil::winsize.Y / 2 + 2,StringColor::green,"");
    IOUtil::writeWithColor(TodoList::parseToString(TodoList::dateCount),IOUtil::winsize.X / 5 + 16,IOUtil::winsize.Y / 2 + 2,StringColor::green,"");

    IOUtil::writeWithColor("现未完成待办事项：",IOUtil::winsize.X / 5,IOUtil::winsize.Y / 2 + 6,StringColor::green,"");
    IOUtil::writeWithColor(TodoList::parseToString(TodoList::list.size() - TodoList::nowCompleteCount),IOUtil::winsize.X / 5 + 18,IOUtil::winsize.Y / 2 + 6,StringColor::green,"");

    IOUtil::writeWithColor("最长专注时长：",IOUtil::winsize.X / 5,IOUtil::winsize.Y / 2 + 10,StringColor::green,"");
    IOUtil::writeWithColor(TodoList::parseToTime(TodoList::longTime),IOUtil::winsize.X / 5 + 14,IOUtil::winsize.Y / 2 + 10,StringColor::green,"");
}

void OutputUtil::outputOperatorBar() {
    IOUtil::writeWithColor("总数：",IOUtil::winsize.X / 5,3,StringColor::yellow,"");
    IOUtil::writeWithColor(TodoList::parseToString(TodoList::list.size()),IOUtil::winsize.X / 5 + 6,3,StringColor::yellow,"");
    IOUtil::writeWithColor("清单",IOUtil::winsize.X / 2,3,StringColor::green,"");
    IOUtil::writeWithColor("新建",IOUtil::winsize.X * 4 / 5,3,StringColor::red,"");
}

void OutputUtil::outputList() {
    outputOperatorBar();
    if (TodoList::list.empty()) {
        IOUtil::writeWithColor("目前没有待办事项，点击“新建”创建新的待办事项吧！",(IOUtil::winsize.X - 48) / 2,IOUtil::winsize.Y / 2,StringColor::skyblue,"");
        IOUtil::write(clearFull,IOUtil::winsize.X / 5,5);
    }else {
        if (TodoList::sortType == 0) IOUtil::writeWithColor("标题",IOUtil::winsize.X / 5,5,StringColor::skyblue,"");
        else if (TodoList::sortType == 1) IOUtil::writeWithColor("标题",IOUtil::winsize.X / 5,5,StringColor::yellow,"");
        else IOUtil::writeWithColor("标题",IOUtil::winsize.X / 5,5,StringColor::red,"");

        if (TodoList::sortType == 2) IOUtil::writeWithColor("时长",IOUtil::winsize.X * 4 / 5 - 30,5,StringColor::skyblue,"");
        else if (TodoList::sortType == 3) IOUtil::writeWithColor("时长",IOUtil::winsize.X * 4 / 5 - 30,5,StringColor::yellow,"");
        else IOUtil::writeWithColor("时长",IOUtil::winsize.X * 4 / 5 - 30,5,StringColor::red,"");

        if (TodoList::sortType == 4) IOUtil::writeWithColor("状态",IOUtil::winsize.X * 4 / 5 - 12,5,StringColor::skyblue,"");
        else if (TodoList::sortType == 5) IOUtil::writeWithColor("状态",IOUtil::winsize.X * 4 / 5 - 12,5,StringColor::yellow,"");
        else IOUtil::writeWithColor("状态",IOUtil::winsize.X * 4 / 5 - 12,5,StringColor::red,"");

        if (TodoList::sortType == 6) IOUtil::writeWithColor("日期",IOUtil::winsize.X * 4 / 5 - 21,5,StringColor::skyblue,"");
        else if (TodoList::sortType == 7) IOUtil::writeWithColor("日期",IOUtil::winsize.X * 4 / 5 - 21,5,StringColor::yellow,"");
        else IOUtil::writeWithColor("日期",IOUtil::winsize.X * 4 / 5 - 21,5,StringColor::red,"");
    }

    time_t now = time(nullptr);
    tm* tt = localtime(&now);

    for(int i = TodoList::start;i < TodoList::list.size() && i < TodoList::start + TodoList::up;++i) {
        std::string tmp = TodoList::parseToString(TodoList::list[i].duration) + "sec";

        IOUtil::write(clearLine,IOUtil::winsize.X / 5,7 + (i - TodoList::start) * 2);

        if (TodoList::list[i].title.size() <= 33)
            IOUtil::writeWithColor(TodoList::list[i].title,IOUtil::winsize.X / 5,7 + (i - TodoList::start) * 2,StringColor::skyblue,"");
        else
            IOUtil::writeWithColor(TodoList::list[i].title.substr(0,33) + "...",IOUtil::winsize.X / 5,7 + (i - TodoList::start) * 2,StringColor::skyblue,"");

        IOUtil::writeWithColor(TodoList::list[i].date,IOUtil::winsize.X * 4 / 5 - 24,7 + (i - TodoList::start) * 2,StringColor::blue,"");

        IOUtil::writeWithColor(tmp,IOUtil::winsize.X * 4 / 5 - 25 - tmp.size(),7 + (i - TodoList::start) * 2,StringColor::purple,"");

        if (TodoList::list[i].complete) {
            IOUtil::writeWithColor("已完成",IOUtil::winsize.X * 4 / 5 - 13,7 + (i - TodoList::start) * 2,StringColor::yellow,"");
        } else if (tt->tm_year + 1900 > TodoList::list[i].year || (tt->tm_year + 1900 == TodoList::list[i].year && (tt->tm_mon + 1 > TodoList::list[i].month || (tt->tm_mon + 1 == TodoList::list[i].month && tt->tm_mday > TodoList::list[i].day)))) {
            IOUtil::writeWithColor("已逾期",IOUtil::winsize.X * 4 / 5 - 13,7 + (i - TodoList::start) * 2,StringColor::red,"");
        } else {
            IOUtil::writeWithColor("未完成",IOUtil::winsize.X * 4 / 5 - 13,7 + (i - TodoList::start) * 2,StringColor::white,"");
        }

        IOUtil::writeWithColor("专注",IOUtil::winsize.X * 4 / 5 - 5,7 + (i - TodoList::start) * 2,StringColor::red,"");

        if (TodoList::list[i].del) IOUtil::writeWithColor("确认",IOUtil::winsize.X * 4 / 5,7   + (i - TodoList::start) * 2,"",StringColor::bred);
        else IOUtil::writeWithColor("删除",IOUtil::winsize.X * 4 / 5,7   + (i - TodoList::start) * 2,StringColor::red,"");
    }
}

void OutputUtil::outputNewPage() {
    clear();
    IOUtil::writeWithColor("标题：",IOUtil::winsize.X / 5,5,StringColor::green,"");
    TodoItem ti;
    std::cin >> ti.title;
    std::string tmp;
    while(true) {
        IOUtil::write(clearFull,IOUtil::winsize.X / 5,7);
        IOUtil::writeWithColor("专注时长（单位为秒，最多10800秒）：",IOUtil::winsize.X / 5,7,StringColor::green,"");
        std::cin >> tmp;
        int n = tmp.size();
        ti.duration = 0;
        for(int i = 0;i < n;++i) {
            if (tmp[i] >= '0' && tmp[i] <= '9') ti.duration = ti.duration * 10 + tmp[i] - '0';
            else {
                ti.duration = 0;
                break;
            }
        }
        if (ti.duration <= 10800 && ti.duration > 0) break;
        IOUtil::writeWithColor("请输入正确的时长，不得小于等于0，不得大于10800",IOUtil::winsize.X / 2 - 23,IOUtil::winsize.Y - 3,StringColor::red,"");
    }
    IOUtil::write(clearFull,IOUtil::winsize.X / 2 - 23,IOUtil::winsize.Y - 3);
    std::regex r("\\d{4}/\\d{2}/\\d{2}");
    bool f = true;
    while(true) {
        IOUtil::write(clearFull,IOUtil::winsize.X / 5,9);
        if (f) f = false;
        else {
            IOUtil::writeWithColor("请输入正确的日期格式：yyyy/MM/dd",IOUtil::winsize.X / 2 - 16,IOUtil::winsize.Y - 3,StringColor::red,"");
        }
        IOUtil::writeWithColor("截止日期（如:2024/02/12）：",IOUtil::winsize.X / 5,9,StringColor::green,"");
        std::cin >> ti.date;
        if (!std::regex_match(ti.date,r)) continue;

        ti.year = (ti.date[0] - '0') * 1000 + (ti.date[1] - '0') * 100 + (ti.date[2] - '0') * 10 + (ti.date[3] - '0');
        ti.month = (ti.date[5] - '0') * 10 + (ti.date[6] - '0');
        ti.day = (ti.date[8] - '0') * 10 + (ti.date[9] - '0');

        if (ti.month > 12 || ti.month <= 0) continue;
        if (ti.month == 1 || ti.month == 3 || ti.month == 5 || ti.month == 7 || ti.month == 8 || ti.month == 10 || ti.month == 12) {
            if (ti.day == 0 || ti.day > 31) {
                continue;
            }
        }
        if (ti.month == 4 || ti.month == 6 || ti.month == 9 || ti.month == 11) {
            if (ti.day == 0 || ti.day > 30) continue;
        }
        if (ti.month == 2) {
            if (ti.year % 4 == 0 && (ti.year % 100 != 0 || ti.year % 400 == 0)) {
                if (ti.day == 0 || ti.day > 29) continue;
            } else {
                if (ti.day == 0 || ti.day > 28) continue;
            }
        }

        break;
    }
    IOUtil::write(clearFull,IOUtil::winsize.X / 2 - 16,IOUtil::winsize.Y - 3);

    IOUtil::writeWithColor("内容描述：",IOUtil::winsize.X / 5,11,StringColor::green,"");
    getchar();
    std::getline(std::cin,ti.description);
    TodoList::list.push_back(ti);
    TodoList::sumCount++;
}

void OutputUtil::outputAttation(int x,int id) {
    clear();
    IOUtil::writeWithColor(TodoList::list[x].title,(IOUtil::winsize.X - TodoList::list[x].title.size()) / 2,5,StringColor::skyblue,"");
    if (TodoList::list[x].description.size()) IOUtil::write("内容描述：" + TodoList::list[x].description,4,8);
    else IOUtil::write("内容描述：无",4,8);
    IOUtil::writeWithColor(TodoList::parseToTime(TodoList::list[x].duration),IOUtil::winsize.X / 2 - 4,IOUtil::winsize.Y / 2,StringColor::green,"");
    for(int i = 0;i < IOUtil::winsize.X && Listener::stage != 1;++i) {
        IOUtil::write("-",i,IOUtil::winsize.Y - 6);
    }
    for(int i = 1;i <= TodoList::list[x].duration && Listener::stage == 3 && TodoList::id == id;++i) {
        if (Listener::stage != 3 || TodoList::id != id) return;
        Sleep(1000);
        if (Listener::stage != 3 || TodoList::id != id) return;
        if (i <= 0.5 * TodoList::list[x].duration) IOUtil::writeWithColor(TodoList::parseToTime(TodoList::list[x].duration - i),IOUtil::winsize.X / 2 - 4,IOUtil::winsize.Y / 2,StringColor::green,"");
        else if (i <= 0.75 * TodoList::list[x].duration) IOUtil::writeWithColor(TodoList::parseToTime(TodoList::list[x].duration - i),IOUtil::winsize.X / 2 - 4,IOUtil::winsize.Y / 2,StringColor::yellow,"");
        else IOUtil::writeWithColor(TodoList::parseToTime(TodoList::list[x].duration - i),IOUtil::winsize.X / 2 - 4,IOUtil::winsize.Y / 2,StringColor::red,"");
        if (Listener::stage != 3 || TodoList::id != id) return;
        for(int j = 0;j < i * IOUtil::winsize.X / TodoList::list[x].duration && Listener::stage == 3 && TodoList::id == id;++j) {
            if (Listener::stage != 3 || TodoList::id != id) return;
            IOUtil::writeWithColor("#",j,IOUtil::winsize.Y - 6,StringColor::purple,"");
        }
    }
    if (Listener::stage != 3 || TodoList::id == id) return;
    TodoList::sumTime += TodoList::list[x].duration;
    TodoList::completeCount++;

    if (!TodoList::list[x].complete) {
        TodoList::longTime = std::max(TodoList::longTime,TodoList::list[x].duration);
        TodoList::nowCompleteCount++;
    }

    TodoList::list[x].complete = true;
    IOUtil::writeToFile();
    Sleep(1000);
    IOUtil::writeWithColor("专注结束，当前待办事项已完成！",IOUtil::winsize.X / 2 - 15,IOUtil::winsize.Y / 2,StringColor::red,"");
    TodoList::id++;
}


void OutputUtil::clear() {
    DWORD res;
    FillConsoleOutputCharacter(IOUtil::hout, ' ', IOUtil::buflen, IOUtil::startpos, &res);
    IOUtil::move(0, 0);
}
