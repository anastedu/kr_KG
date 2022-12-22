//
// Created by derfreimensch on 27/11/2021.
//

#include "Polygon.h"

std::vector<Matrix>* Polygon::getPoints() {
    return &points;
}

int* Polygon::getEqn() {
    std::vector<Matrix> ptr = Polygon::getPoints()[0];
    int y1 = ptr[0].getByRowCol(0, 1);
    int y2 = ptr[1].getByRowCol(0, 1);
    int y3 = ptr[2].getByRowCol(0, 1);
    int x1 = ptr[0].getByRowCol(0, 0);
    int x2 = ptr[1].getByRowCol(0, 0);
    int x3 = ptr[2].getByRowCol(0, 0);
    int z1 = ptr[0].getByRowCol(0, 2);
    int z2 = ptr[1].getByRowCol(0, 2);
    int z3 = ptr[2].getByRowCol(0, 2);

    delete[] equation;
    equation = new int[4];
    equation[0] = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
    equation[1] = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1);
    equation[2] = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
    int tmp = 0;

    tmp += (-x1) * equation[0];
    tmp += (-y1) * equation[1];
    tmp += (-z1) * equation[2];
    equation[3] = tmp;

    return equation;
}

float Polygon::getZat(float x, float y) {
    int* eqn = this->getEqn();
    return (x * eqn[0] + y * eqn[1] + eqn[3]) / (-eqn[2]);
}

float Polygon::getMidZofClosestLine() {
    if (isShadow) {
        return 1000000000.0;
    }
    float res1 = points[0].getByRowCol(0, 2) + points[1].getByRowCol(0, 2);
    float res2 = points[0].getByRowCol(0, 2) + points[2].getByRowCol(0, 2);
    float res3 = points[1].getByRowCol(0, 2) + points[2].getByRowCol(0, 2);
    return std::min(std::min(res1 / 2, res2 / 2), res3 / 2);
}

Polygon Polygon::getShadow(Matrix& LightSource, float maxY) {
    Polygon res(*this);
    float xl = LightSource.getByRowCol(0, 0);
    float yl = LightSource.getByRowCol(0, 1);
    float zl = LightSource.getByRowCol(0, 2);

    for (auto& i : res.points) {
        float x = i.getByRowCol(0, 0);
        float y = i.getByRowCol(0, 1);
        float z = i.getByRowCol(0, 2);
        float f = yl;
        if (f == 0.0f) f = 1;
        x = x - (xl * (y - maxY)) / f;
        z = z - (zl * (y - maxY)) / f;
        y = maxY;
        float mas[4] = { x, y, z, 1 };
        i.fill(mas, 4);
    }
    res.isShadow = true;
    res.color = 15;
    return res;
}

void Polygon::rotate(int angle, Matrix& rotPoint, char axis) {
    float rotX = rotPoint.getByRowCol(0, 0);
    float rotY = rotPoint.getByRowCol(0, 1);
    float rotZ = rotPoint.getByRowCol(0, 2);
    Matrix move = generate3DMoveMatrix(-rotX, -rotY, -rotZ);
    Matrix negMove = generate3DMoveMatrix(rotX, rotY, rotZ);
    Matrix rot = generate3DRotationMatrix(angle, axis);
    for (auto& point : points) {
        point = point * move * rot * negMove;
    }
}

void Polygon::scale(float size, Matrix scalePoint) {
    float rotX = scalePoint.getByRowCol(0, 0);
    float rotY = scalePoint.getByRowCol(0, 1);
    float rotZ = scalePoint.getByRowCol(0, 2);
    Matrix move = generate3DMoveMatrix(-rotX, -rotY, -rotZ);
    Matrix negMove = generate3DMoveMatrix(rotX, rotY, rotZ);
    Matrix scale = generate3DScaleMatrix(size);
    for (auto& point : points) {
        point = point * move * scale * negMove;
    }
}

void Polygon::move(int x, int y, int z) {
    Matrix move = generate3DMoveMatrix(x, y, z);
    for (auto& point : points) {
        point = point * move;
    }
}

void Polygon::convertToScreenCoords() {
    Matrix projection(4, 4);
    float data[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 1.0 / 2,  -1.0 / 2, 1, 0, 0, 0, 0, 1 };
    projection.fill(data, 16);
    for (auto& point : points) {
        point = point * projection;
    }
    getEqn();
}

Polygon::Polygon(Matrix& pt1, Matrix& pt2, Matrix& pt3, bool isSh, int c) {
    color = c;
    isShadow = isSh;
    points.emplace_back(pt1);
    points.emplace_back(pt2);
    points.emplace_back(pt3);
    equation = nullptr;
}

Polygon::Polygon(Matrix& pt1, Matrix& pt2, Matrix& pt3, Matrix& pt4, bool isSh, int c) {
    color = c;
    isShadow = isSh;
    points.emplace_back(pt1);
    points.emplace_back(pt2);
    points.emplace_back(pt3);
    points.emplace_back(pt4);
    equation = nullptr;
}

Polygon::Polygon(Polygon const& from) {
    isShadow = from.isShadow;
    color = from.color;
    for (const auto& point : from.points) {
        points.emplace_back(point);
    }
    equation = nullptr;
}

int Polygon::getAmtOfPointsInsidePoly(std::vector<int>& xp, std::vector<int>& yp, bool earlyStop) {
    getEqn();
    int res = 0;
    for (auto& x : xp) {
        for (auto& y : yp) {
            bool c = false;
            for (int i = 0, j = points.size() - 1; i < points.size(); j = i++) {
                int yi = points[i].getByRowCol(0, 1);
                int yj = points[j].getByRowCol(0, 1);
                int xi = points[i].getByRowCol(0, 0);
                int xj = points[j].getByRowCol(0, 0);
                if (((yi > y) != (yj > y)) && (x < (xj - xi) * (y - yi) / (yj - yi) + xi)) {
                    c = !c;
                }
            }
            if (c && earlyStop) return 1;
            res += c;
        }
    }
    return res;
}

int Polygon::getColor() const {
    return color;
}

bool Polygon::operator==(const Polygon& rhs) const {
    return points == rhs.points &&
        equation == rhs.equation &&
        isShadow == rhs.isShadow &&
        color == rhs.color;
}

bool Polygon::operator!=(const Polygon& rhs) const {
    return !(rhs == *this);
}

bool Polygon::ccw() {
    float ax = points[0].getByRowCol(0, 0), ay = points[0].getByRowCol(0, 1);
    float bx = points[2].getByRowCol(0, 0), by = points[2].getByRowCol(0, 1);
    float cx = points[1].getByRowCol(0, 0), cy = points[1].getByRowCol(0, 1);

    float a = bx - ax;
    float b = by - ay;
    float c = cx - ax;
    float d = cy - ay;

    float signed_area = a * d - b * c;
    return signed_area > 0;
}

bool Polygon::isShadow1() const {
    return isShadow;
}

