#include "UniqueRngPoint.h"
#include <cmath>

UniqueRngPoint::UniqueRngPoint(float width, float height) : x_max(width), y_max(height) {}

//Gives unique random points in specified for the object range.
void UniqueRngPoint::operator()(Point* p) {
	Point temp_point;
	do {
        temp_point.x = random_float(0, this->x_max);
		temp_point.y = random_float(0, this->y_max);
	} while (!point_is_unique(temp_point));
	p->x = temp_point.x;
	p->y = temp_point.y;
	points.push_back(temp_point);
}

bool UniqueRngPoint::point_is_unique(const Point point) {
	for(auto &p : points)
		if(point.x == p.x && point.y == p.y)
			return false;
	return true;
}

float UniqueRngPoint::random_float(float lower, float upper) {
	float integral;
    float fraqtion = modff(upper + lower, &integral);

	return (float)(rand() % (long long)integral) + fraqtion + (float)(rand() % 10000) / 10000 + lower;
}
