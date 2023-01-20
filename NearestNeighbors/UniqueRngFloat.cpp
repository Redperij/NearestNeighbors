#include "UniqueRngPoint.h"
#include <cmath>

UniqueRngPoint::UniqueRngPoint(float min, float max) : lower(min), upper(max) {}

//Gives unique random numbers in specified for the object range.
float UniqueRngPoint::operator()() {
	float number;
    float integral;
    float fraqtion = modff(this->upper + this->lower, &integral);
	do {
        number = (float) (rand() % (long long)integral) + fraqtion + (float)(rand() % 10000) / 10000 + this->lower;
	} while (find(numbers.begin(), numbers.end(), number) != numbers.end());
	numbers.push_back(number);
	return number;
}
