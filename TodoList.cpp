#include "TodoList.h"

#include <algorithm>
#include <string>
#include <vector>

int TodoList::start = 0;
int TodoList::up;
int TodoList::now = 0;
int TodoList::sortType = 0;
int TodoList::sumCount = 0;
int TodoList::sumTime = 0;
int TodoList::completeCount = 0;
int TodoList::dateCount = 0;
int TodoList::longTime = 0;
int TodoList::nowItem = 0;
int TodoList::nowCompleteCount = 0;
int TodoList::id = 0;
std::vector<TodoItem> TodoList::list;
std::string TodoList::parseToString(int x) {
    std::string res,tmp;
    if (x == 0) tmp.push_back('0');
    while(x) {
        tmp.push_back(x % 10 + '0');
        x /= 10;
    }
    for(int i = tmp.size() -1 ;i >= 0;--i) {
        res.push_back(tmp[i]);
    }
    return res;
}

std::string TodoList::parseToTime(int x) {
    std::string res;
    res.push_back(x / 60 / 60 / 10 + '0');
    res.push_back(x / 60 / 60 % 10 + '0');
    res.push_back(':');
    res.push_back(x % 3600 / 60 / 10 + '0');
    res.push_back(x % 3600 / 60 % 10 + '0');
    res.push_back(':');
    res.push_back(x % 60 / 10 + '0');
    res.push_back(x % 60 % 10 + '0');
    return res;
}

void TodoList::sortList() {
    switch(sortType) {
        case 0:
            std::sort(list.begin(),list.end(),[](TodoItem &i1,TodoItem &i2) {
                return i1.title < i2.title;
            });
            break;
        case 1:
            std::sort(list.begin(),list.end(),[](TodoItem &i1,TodoItem &i2) {
                return i1.title > i2.title;
            });
            break;
        case 2:
            std::sort(list.begin(),list.end(),[](TodoItem &i1,TodoItem &i2) {
                return i1.duration < i2.duration;
            });
            break;
        case 3:
            std::sort(list.begin(),list.end(),[](TodoItem &i1,TodoItem &i2) {
                return i1.duration > i2.duration;
            });
            break;
        case 4:
            std::sort(list.begin(),list.end(),[](TodoItem &i1,TodoItem &i2) {
                return i1.complete < i2.complete;
            });
            break;
        case 5:
            std::sort(list.begin(),list.end(),[](TodoItem &i1,TodoItem &i2) {
                return i1.complete > i2.complete;
            });
            break;
        case 6:
            std::sort(list.begin(),list.end(),[](TodoItem &i1,TodoItem &i2) {
                if (i1.year != i2.year) return i1.year < i2.year;
                if (i1.month != i2.month) return i1.month < i2.month;
                return i1.day < i2.day;
            });
            break;
        case 7:
            std::sort(list.begin(),list.end(),[](TodoItem &i1,TodoItem &i2) {
                if (i1.year != i2.year) return i1.year > i2.year;
                if (i1.month != i2.month) return i1.month > i2.month;
                return i1.day > i2.day;
            });
            break;
        default:
            break;
    }
}


