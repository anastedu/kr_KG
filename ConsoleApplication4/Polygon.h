//
// Created by derfreimensch on 27/11/2021.
//

#ifndef CGCOURSE_POLYGON_H
#define CGCOURSE_POLYGON_H

#ifndef MATRIX_H
#include "matrix.h"
#endif

#include <vector>

class Polygon {
    std::vector<Matrix> points;
    int* equation{};
    bool isShadow{};
    int color{};
public:
    Polygon() = default;
    Polygon(Matrix& pt1, Matrix& pt2, Matrix& pt3, Matrix& pt4, bool isSh = false, int c = 0);
    Polygon(Matrix& pt1, Matrix& pt2, Matrix& pt3, bool isSh = false, int c = 0);
    Polygon(Polygon const& from);
    ~Polygon() = default;
    int getColor() const;
    int* getEqn();
    float getZat(float x, float y);
    float getMidZofClosestLine();
    int getAmtOfPointsInsidePoly(std::vector<int>& xp, std::vector<int>& yp, bool earlyStop = false); // ������� ���������� ����� ���� ������ ��������
    Polygon getShadow(Matrix& LightSource, float maxY);
    void rotate(int angle, Matrix& rotPoint, char axis);
    void scale(float size, Matrix scalePoint);
    void move(int x, int y, int z);
    void convertToScreenCoords();
    std::vector<Matrix>* getPoints();
    bool isShadow1() const;
    bool operator==(const Polygon& rhs) const;
    bool operator!=(const Polygon& rhs) const;
    bool ccw();
};


#endif //CGCOURSE_POLYGON_H
