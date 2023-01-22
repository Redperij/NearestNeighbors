#ifndef TEST_H_
#define TEST_H_

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "NearestNeighbors.h"

int run_tests();
int run_test_with_valid_points(Point* points, int size,
    int image_width, int image_height, const char* test_name);
int run_test(Point* points, int size, int image_width,
             int image_height, const char* test_name);
int run_custom_test(Point* points, int size, int image_width,
                    int image_height, const char* test_name);
void generate_valid_unique_points(Point* points, size_t size,
                                  int image_width, int image_height);
void generate_unique_points(Point* points, size_t size);
Point* shortest_dist(Point* p, std::vector<Point*> points);
bool point_in_bounds(Point* p, size_t max_x, size_t max_y);

#endif // !TEST_H_
