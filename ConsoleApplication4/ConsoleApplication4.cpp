﻿#include <iostream>
#include <vector>
#include "Polygon.h"
#include "Figure.h"
#include "window.h"
#include "graphics.h"
#include <algorithm>
#include <set>
#define MINWIN 1

using namespace std;

std::vector<Figure> init();
void fill(int color, int xScr1 = 0, int xScr2 = getmaxx(), int yScr1 = 0, int yScr2 = getmaxy());
void draw(std::vector<Figure>& figures, int x1, int y1, int x2, int y2);

int WinMain() {
    initwindow(600, 600);
    sdlbgiauto();
    std::vector<Figure> figs = init();
    draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
    int curFig = 0;
    while (true) {
        char c = getch();
        if (c == ' ') {
            closegraph();
            return 0;
        }
        else {
            Matrix rotPoint = figs[curFig].getMidPoint();
            switch (c) {
            case 'q':
                figs[curFig].move(0, 0, -50);
                setbkcolor(0);
                draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                break;
            case 'e':
                figs[curFig].move(0, 0, 50);
                setbkcolor(0);
                draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                break;
            case 'w':
                figs[curFig].move(0, -50, 0);
                setbkcolor(0);
                draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                break;
            case 'a':
                figs[curFig].move(-50, 0, 0);
                setbkcolor(0);
                draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                break;
            case 's':
                figs[curFig].move(0, 50, 0);
                setbkcolor(0);
                draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                break;
            case 'd':
                figs[curFig].move(50, 0, 0);
                setbkcolor(0);
                draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                break;
            case 'r': {
                figs[curFig].rotate(30, rotPoint, 'x');
                setbkcolor(0);
                draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                break;
            }
            case 't': {
                figs[curFig].rotate(-30, rotPoint, 'x');
                setbkcolor(0);
                draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                break;
            }
            case 'f': {
                figs[curFig].rotate(30, rotPoint, 'y');
                setbkcolor(0);
                draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                break;
            }
            case 'g': {
                figs[curFig].rotate(-30, rotPoint, 'y');
                setbkcolor(0);
                draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                break;
            }
            case 'v': {
                figs[curFig].rotate(30, rotPoint, 'z');
                setbkcolor(0);
                draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                break;
            }
            case 'b': {

                figs[curFig].rotate(-30, rotPoint, 'z');
                setbkcolor(0);
                draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                break;
            }
            case 'l':
                curFig += 1;
                curFig %= figs.size();
                break;
            case 'z': {
                figs[curFig].scale(0.5, rotPoint);
                setbkcolor(0);
                draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                break;
            }
            case 'c': {
                figs[curFig].scale(2, rotPoint);
                setbkcolor(0);
                draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                break;
            }
            default:
                break;
            }
        }
    }
}

void fill(int color, int xScr1, int xScr2, int yScr1, int yScr2) {
    for (int x = xScr1; x <= xScr2; x++) {
        setcolor(color);
        line(x, yScr1, x, yScr2);
    }
    setcolor(15);
}

bool comp(Polygon& a, Polygon& b) {
    return a.getMidZofClosestLine() < b.getMidZofClosestLine();
}

void draw(std::vector<Figure>& figures, int x1, int y1, int x2, int y2) {
    vector<Window>stack;
    Window fst(x1, y1, x2, y2);
    stack.push_back(fst);
    vector<Polygon> unsorted;
    Matrix lSource(1000, -1000, 0);
    for (auto& figure : figures) {
        for (int j = 0; j < figure.getPolys()->size(); j++) {
            unsorted.emplace_back(figure.getPolys()[0][j].getShadow(lSource, getmaxy() + 1));
            unsorted[unsorted.size() - 1].convertToScreenCoords();
            unsorted[unsorted.size() - 1].getEqn();
            if (!unsorted[unsorted.size() - 1].ccw()) unsorted.pop_back();
        }
    }
    for (auto& figure : figures) {
        for (int j = 0; j < figure.getPolys()->size(); j++) {
            unsorted.emplace_back(figure.getPolys()[0][j]);
            unsorted[unsorted.size() - 1].convertToScreenCoords();
            auto eqn = unsorted[unsorted.size() - 1].getEqn();
            std::cout << eqn[0] << ' ' << eqn[1] << ' ' << eqn[2] << ' ' << eqn[3] << endl;
            if (!unsorted[unsorted.size() - 1].ccw()) unsorted.pop_back();
        }
    }
    sort(unsorted.begin(), unsorted.end(), comp);
    int cnt = 0;
    while (!stack.empty()) {
        Window win = stack[stack.size() - 1];
        stack.pop_back();
        if (win.size < 1) continue;
        int xStart = win.x1;
        int yStart = win.y1;
        int xEnd = win.x2;
        int yEnd = win.y2;
        //        setcolor(1);
        //        line(xStart, yStart, xEnd, yStart);
        //        line(xEnd, yStart, xEnd, yEnd);
        //        line(xEnd, yEnd, xStart, yEnd);
        //        line(xStart, yEnd, xStart, yStart);
        //        setcolor(15);
        vector<Polygon> in;
        vector<Polygon> out;
        vector<Polygon> intersect;
        vector<Polygon> overlap;
        vector<int> xs = { xStart, xEnd };
        vector<int> ys = { yStart, yEnd };
        vector<int> xAll;
        for (int i = xStart; i < xEnd; i++) {
            xAll.emplace_back(i);
        }
        vector<int> yAll;
        for (int i = yStart; i < yEnd; i++) {
            yAll.emplace_back(i);
        }
        for (auto& poly : unsorted) {
            int tmp = poly.getAmtOfPointsInsidePoly(xs, ys);
            if (tmp == 4) {
                overlap.emplace_back(poly);
            }
            else if (tmp < 4 && tmp > 0) {
                intersect.emplace_back(poly);
            }
            else {
                int ptsInWin = 0;
                vector<Matrix> pts = poly.getPoints()[0];
                for (auto& pt : pts) {
                    float x = pt.getByRowCol(0, 0);
                    float y = pt.getByRowCol(0, 1);
                    if (x >= xStart && x < xEnd && y >= yStart && y < yEnd) {
                        ptsInWin++;
                    }
                }
                if (ptsInWin == 0) {
                    if (poly.getAmtOfPointsInsidePoly(xAll, yAll, true) > 0) intersect.emplace_back(poly);
                    else out.emplace_back(poly);
                }
                else if (ptsInWin == pts.size()) {
                    in.emplace_back(poly);
                }
                else {
                    intersect.emplace_back(poly);
                }
            }
        }
        if (out.size() == unsorted.size()) {
            continue;
        }
        if (win.size == MINWIN) {
            vector<Polygon> tmp;
            tmp.reserve(overlap.size() + in.size() + intersect.size());
            for (auto& i : overlap) {
                tmp.emplace_back(i);
            }
            for (auto& i : in) {
                tmp.emplace_back(i);
            }
            for (auto& i : intersect) {
                tmp.emplace_back(i);
            }
            float z = tmp[0].getZat(xStart, yStart);
            int color = tmp[0].getColor();
            for (auto& poly : tmp) {
                if (poly.isShadow1()) continue;
                if (poly.getZat(xStart, yStart) < z) {
                    color = poly.getColor();
                    //                    fill((xStart + xEnd) / 2, (yStart + yEnd) / 2, color, xStart, xEnd - 1, yStart, yEnd - 1);
                    z = poly.getZat(xStart, yStart);
                }
            }
            fill(color, xStart, xEnd - 1, yStart, yEnd - 1);
            //            putpixel(xStart, yStart, tmp[0].getColor());
            continue;
        }
        if (!intersect.empty() || !in.empty()) {
            if (win.size > MINWIN) {
                stack.emplace_back(xStart, yStart, (xStart + xEnd) / 2, (yStart + yEnd) / 2);
                stack.emplace_back((xStart + xEnd) / 2, (yStart + yEnd) / 2, xEnd, yEnd);
                stack.emplace_back(xStart, (yStart + yEnd) / 2, (xStart + xEnd) / 2, yEnd);
                stack.emplace_back((xStart + xEnd) / 2, yStart, xEnd, (yStart + yEnd) / 2);
            }
            continue;
        }
        if (!overlap.empty()) {
            float zAt1 = overlap[0].getZat(xStart, yStart);
            float zAt2 = overlap[0].getZat(xEnd - 1, yStart);
            float zAt3 = overlap[0].getZat(xEnd - 1, yEnd - 1);
            float zAt4 = overlap[0].getZat(xStart, yEnd - 1);
            int color1 = overlap[0].getColor();
            int color2 = overlap[0].getColor();
            int color3 = overlap[0].getColor();
            int color4 = overlap[0].getColor();
            for (auto& poly : overlap) {
                if (poly.isShadow1()) continue;
                if (zAt1 > poly.getZat(xStart, yStart)) {
                    zAt1 = poly.getZat(xStart, yStart);
                    color1 = poly.getColor();
                }
                if (zAt2 > poly.getZat(xEnd - 1, yStart)) {
                    color2 = poly.getColor();
                    zAt2 = poly.getZat(xEnd - 1, yStart);
                }
                if (zAt3 > poly.getZat(xEnd - 1, yEnd - 1)) {
                    color3 = poly.getColor();
                    zAt3 = poly.getZat(xEnd - 1, yEnd - 1);
                }
                if (zAt4 > poly.getZat(xStart, yEnd - 1)) {
                    color4 = poly.getColor();
                    zAt4 = poly.getZat(xStart, yEnd - 1);
                }
            }
            set<int>colors = { color1, color2, color3, color4 };
            if (colors.size() > 1) {
                if (xEnd - xStart <= 1 && yEnd - yStart <= 1) {
                    continue;
                }
                if (win.size > MINWIN) {
                    stack.emplace_back(xStart, yStart, (xStart + xEnd) / 2, (yStart + yEnd) / 2);
                    stack.emplace_back((xStart + xEnd) / 2, (yStart + yEnd) / 2, xEnd, yEnd);
                    stack.emplace_back(xStart, (yStart + yEnd) / 2, (xStart + xEnd) / 2, yEnd);
                    stack.emplace_back((xStart + xEnd) / 2, yStart, xEnd, (yStart + yEnd) / 2);
                }
                continue;
            }
            fill(color1, xStart, xEnd - 1, yStart, yEnd - 1);
            cnt++;
        }
    }
}

std::vector<Figure> init() {
    std::vector<Figure>res;
    std::vector<Matrix>pts;
    pts.emplace_back(Matrix(400, 400, 100));
    pts.emplace_back(Matrix(500, 400, 100));
    pts.emplace_back(Matrix(400, 400, 200));
    pts.emplace_back(Matrix(500, 400, 200));
    pts.emplace_back(Matrix(400, 300, 100));
    pts.emplace_back(Matrix(500, 300, 100));
    pts.emplace_back(Matrix(400, 300, 200));
    pts.emplace_back(Matrix(500, 300, 200));
    res.emplace_back(Figure(pts, 2));
    pts.clear();

    pts.emplace_back(Matrix(50, 500, 100));
    pts.emplace_back(Matrix(150, 500, 100));
    pts.emplace_back(Matrix(50, 500, 300));
    pts.emplace_back(Matrix(150, 500, 300));
    pts.emplace_back(Matrix(50, 250, 100));
    pts.emplace_back(Matrix(150, 250, 100));
    pts.emplace_back(Matrix(50, 250, 300));
    pts.emplace_back(Matrix(150, 250, 300));

    res.emplace_back(Figure(pts, 2));
    return res;
}
