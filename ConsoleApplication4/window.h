//
// Created by derfreimensch on 28/11/2021.
//

#ifndef CGCOURSE_WINDOW_H
#define CGCOURSE_WINDOW_H


class Window {
public:
    int x1, y1, x2, y2, size;
    Window(int x1, int y1, int x2, int y2);

};

struct Point {
    Point(int i, int i1) : x(i), y(i1) {};
    Point() = default;
    int x, y;
};

#endif //CGCOURSE_WINDOW_H
