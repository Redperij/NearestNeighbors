#ifndef UniqueRngPoint_H_
#define UniqueRngPoint_H_

#include <vector>

class UniqueRngPoint {
public:
    UniqueRngPoint(float min = 0, float max = 4096);
    float operator()();
private:
    float lower;
    float upper;
    std::vector<float> numbers;
};

#endif // !UniqueRngPoint_H_