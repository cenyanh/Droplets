#ifndef TODOLIST_H
#define TODOLIST_H
#include <string>
#include <vector>

struct TodoItem {
    std::string title;
    std::string description;
    int duration = 0;
    int complete = false;
    int del = false;
    int year = 2024;
    int month = 8;
    int day = 13;
    int log = 0;
    std::string date = "2024/8/13";
    TodoItem() = default;
    TodoItem(std::string t,std::string desc,int d,bool com):title(t),description(desc),duration(d),complete(com){}
};

class TodoList {
public:
    static int start;
    static int up;
    static int now;
    static int sortType;
    static int sumTime;
    static int sumCount;
    static int completeCount;
    static int dateCount;
    static int longTime;
    static int nowItem;
    static int nowCompleteCount;
    static int id;
    static std::vector<TodoItem> list;
    static std::string parseToString(int x);
    static std::string parseToTime(int x);
    static void sortList();
};



#endif //TODOLIST_H
