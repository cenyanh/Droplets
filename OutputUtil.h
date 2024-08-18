#ifndef OUTPUTUTIL_H
#define OUTPUTUTIL_H
#include <windows.h>

class OutputUtil {
public:
    static std::string title[6];
    static std::string clearLine;
    static std::string clearFull;
    static std::string clearCount;

    void static init(SHORT X, SHORT Y);

    void static outputMainTitle(SHORT x, SHORT y);

    void static outputLoadBar(SHORT y);

    void static outputMainPage();

    void static outputOperatorBar();

    void static outputList();

    void static outputNewPage();

    void static outputAttation(int x,int id);

    void static outputOperatorItem();

    void static outputOperatorPage();

    void static outputInformation();

    void static clear();

};
#endif //OUTPUTUTIL_H
