#ifndef LISTENER_H
#define LISTENER_H


class Listener {
public:
    static const int up;
    static const int down;
    static const int left;
    static const int right;
    static const int esc;
    static const int space;
    static const int a;
    static const int d;
    static const int enter;
    static int stage;
    static unsigned long long last;
    static int lastItem;

    static void mouseClickListener(COORD pos);

    static void mouseMoveListener(COORD pos);

    static void mouseWheelListener(DWORD state);

    static void keyListener(KEY_EVENT_RECORD &key);
};


#endif //LISTENER_H
