#ifndef TEST_H_
#define TEST_H_

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "NearestNeighbors.h"

int run_tests();
void run_old_test();
void generate_rand_points(Point *points, int &n);
void generate_points(Point* points, size_t size);
void generate_int_points(Point* points, size_t size);
void generate_valid_unique_points(Point* points, size_t size, int image_width, int image_height);
void generate_unique_points(Point* points, size_t size);

#endif // !TEST_H_
