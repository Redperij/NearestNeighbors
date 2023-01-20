#ifndef UNIQUERNGPOINT_H_
#define UNIQUERNGPOINT_H_

#include <vector>
#include "NearestNeighbors.h"

class UniqueRngPoint {
public:
    UniqueRngPoint(float width = 4096, float height = 4096);
    void operator()(Point* p);
private:
    float x_max;
    float y_max;
    std::vector<Point> points;
    bool point_is_unique(const Point point);
    float random_float(float lower, float upper);
};

#endif // !UNIQUERNGPOINT_H_
