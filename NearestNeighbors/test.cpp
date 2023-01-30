#include <cmath>
#include <chrono>
#include <iomanip>
#include "test.h"
#include "UniqueRngPoint.h"

#define TEST_CORRECTNESS 1

using namespace std;

/**
 * @brief Runs created tests.
 * 
 * @return int amount of failed tests.
 */
int run_tests() {
    Point* points = new Point[4096];
    int tests_failed = 0;

    //Loop that performs tests.
    for (size_t i = 0; i < 5; i++)
    {
        //Example of test usage.
        tests_failed += run_test_with_valid_points(points, 10, 10, 10, "[Simple valid 10x10]: ");
        tests_failed += run_test(points, 4096, 4096, 4096, "[General 4096 4096x4096]: ");
        tests_failed += run_test_with_valid_points(points, 4096, 4096, 4096, "[Valid 4096 4096x4096]: ");
        tests_failed += run_test_with_valid_points(points, 4096, 1, 1, "[Tightly together]: ");
        tests_failed += run_test(points, 4096, 1, 1, "[Lots of invalid]: ");

        //Example of custom test.
        generate_valid_unique_points(points, 4095, 1, 1);
        points[4095].x = 4095;
        points[4095].y = 4095;

        tests_failed += run_custom_test(points, 4096, 4096, 4096, "[Tight cluster]: ");


        //Example 2 of custom test.
        generate_valid_unique_points(points, 2000, 1, 1);
        generate_valid_unique_points(&points[2000], 2096, 4096, 4096);

        tests_failed += run_custom_test(points, 4096, 4096, 4096, "[Half in tight cluster]: ");
    }

    delete[] points;
    return tests_failed;
}

/**
 * @brief Generates array of random valid unique points and runs test.
 * 
 * @param points Pointer to the allocated memory to use.
 * @param size Amount of points to generate.
 * @param image_width Image width.
 * @param image_height Image height.
 * @param test_name Test name formatted as "[TEST_NAME]: ".
 * @return int 1 - test failed. 0 - test succeeded. 
 */
int run_test_with_valid_points(Point* points, int size, int image_width,
                               int image_height, const char* test_name) {
    generate_valid_unique_points(points, size, image_width, image_height);
    return run_custom_test(points, size, image_width, image_height, test_name);
}

/**
 * @brief Generates array of random unique points and runs test.
 * 
 * @param points Pointer to the allocated memory to use.
 * @param size Amount of points to generate.
 * @param image_width Image width.
 * @param image_height Image height.
 * @param test_name Test name formatted as "[TEST_NAME]: ".
 * @return int 1 - test failed. 0 - test succeeded. 
 */
int run_test(Point* points, int size, int image_width, int image_height,
             const char* test_name) {
    generate_unique_points(points, size);
    return run_custom_test(points, size, image_width, image_height, test_name);
}

/**
 * @brief Runs test with received set of points. (No nullptr check for "points"!)
 * 
 * @param points Set of initialised unique points.
 * @param size Size of an array of points.
 * @param image_width Image width (x).
 * @param image_height Image height (y).
 * @param test_name Test name formatted as "[TEST_NAME]: ".
 * @return int 1 - test failed. 0 - test succeeded.
 */
int run_custom_test(Point* points, int size, int image_width,
                    int image_height, const char* test_name) {
    int res = 0;
    vector<Point*> check_points;
    Point** obj_neighbors = new Point*[size];
    Point** check_neighbors = new Point*[size];

    chrono::steady_clock::time_point start;
    chrono::steady_clock::time_point end;
    chrono::duration<double> time;
    chrono::duration<double> total_time(0);

    size_t missmatch_count = 0;
    
    for (size_t i = 0; i < size; i++)
        check_points.push_back(&points[i]);

    start = chrono::steady_clock::now();
    NearestNeighbors nb(image_width, image_height, points, size);
    end = chrono::steady_clock::now();
    time = end - start;
    total_time += time;
    cout << test_name << "Object initialised for " << time.count() * 1000 << " ms." << endl;

    start = chrono::steady_clock::now();
    for (size_t i = 0; i < size; i++)
        obj_neighbors[i] = nb.getNearestPoint(&points[i]);
    end = chrono::steady_clock::now();
    time = end - start;
    total_time += time;
    cout << test_name << "Object found neighbors for " << time.count() * 1000 << " ms." << endl;

#if TEST_CORRECTNESS
    start = chrono::steady_clock::now();
    for (size_t i = 0; i < size; i++) {
        if (point_in_bounds(&points[i], image_width, image_height))
            check_neighbors[i] = shortest_dist(check_points[i], check_points);
        else
            check_neighbors[i] = nullptr;
    }
    end = chrono::steady_clock::now();
    time = end - start;
    cout << test_name << "Check found neighbors for " << time.count() * 1000 << " ms." << endl;

    for (size_t i = 0; i < size; i++) {
        if (obj_neighbors[i] != check_neighbors[i]) {
            if(obj_neighbors[i] != nullptr && check_neighbors[i] != nullptr) {
                cout << test_name << "MISMATCH: Point (" << points[i].x << "; " << points[i].y
                     << ") has different neighbors.\nObject neighbor: ("
                     << obj_neighbors[i]->x << "; " << obj_neighbors[i]->y << ")\n"
                     "Test neighbor  : (" << check_neighbors[i]->x << "; " << check_neighbors[i]->y << ")" << endl;
                missmatch_count++;
            }
            else if (obj_neighbors[i] != nullptr) {
                cout << test_name << "MISMATCH: Point (" << points[i].x << "; " << points[i].y <<
                     ") has different neighbors.\nObject neighbor: ("
                     << obj_neighbors[i]->x << "; " << obj_neighbors[i]->y << ")\n"
                     "Test neighbor  : Gave a nullptr" << endl;
                missmatch_count++;
            }
            else {
                cout << test_name << "MISMATCH: Point (" << points[i].x << "; " << points[i].y <<
                     ") has different neighbors.\nObject neighbor: Gave a nullptr\n"
                     "Test neighbor  : (" << check_neighbors[i]->x << "; " << check_neighbors[i]->y << ")" << endl;
                missmatch_count++;
            }
            
        }
    }
#endif

    if (missmatch_count) {
        res = 1;
        cout << test_name << "Test completed with errors.\nTotal time spent: "
        << total_time.count() * 1000 << " ms" << endl;
    }
    else
        cout << test_name << "Test successful.\nTotal time spent: "
        << total_time.count() * 1000 << " ms" << endl;

    delete[] obj_neighbors;
    delete[] check_neighbors;

    return res;
}

/**
 * @brief Generates an array of valid unique points.
 * 
 * @param points Pointer to the allocated memory to use.
 * @param size Amount of points to generate.
 * @param image_width Image width.
 * @param image_height Image height.
 */
void generate_valid_unique_points(Point* points, size_t size, int image_width, int image_height) {
    UniqueRngPoint rnd((float)image_width, (float)image_height);
    for (size_t i = 0; i < size; i++)
        rnd(&points[i]);
}

/**
 * @brief Generates an array of unique points.
 * 
 * @param points Pointer to the allocated memory to use.
 * @param size Amount of points to generate.
 */
void generate_unique_points(Point* points, size_t size) {
    UniqueRngPoint rnd(4096, 4096);
    for (size_t i = 0; i < size; i++)
        rnd(&points[i]);
}

/**
 * @brief Uses straightforward linear algorithm to get nearest neighbor.
 * 
 * @param p Point to find neighbor for.
 * @param points Set of points to find neighbor from.
 * @return Point* pointer to the nearest neighbor.
 */
Point* shortest_dist(Point* p, vector<Point*> points) {
    float shortest_dist = 5793; //sqrt(2*4096^2)
    float dx = 0;
    float dy = 0;
    float dist = 0;
    Point* pnearest_neighbor = nullptr;

    for (auto &point : points) {
		//Found self.
		if (p == point)
			continue;
		
		//Get distances for y and x coordinates.
		dx = p->x - point->x;
		dy = p->y - point->y;
		dx = dx >= 0 ? dx : dx * -1;
		dy = dy >= 0 ? dy : dy * -1; 
		
		//Get actual distance.
		if (dx == 0)
			dist = dy;
		else if (dy == 0)
			dist = dx;
		else
			dist = sqrtf(dx * dx + dy * dy);
		
		//Compare it to the shortest one.
		if (shortest_dist > dist) {
			shortest_dist = dist;
			pnearest_neighbor = point;
		}
	}

    return pnearest_neighbor;
}

/**
 * @brief Checks whether point is inside image.
 * 
 * @param p Point to check.
 * @param max_x Image width.
 * @param max_y Image height.
 * @return true point is inside of the image.
 * @return false point is outside of the image.
 */
bool point_in_bounds(Point* p, size_t max_x, size_t max_y) {
	if (p->x < 0 || p->x >= max_x
		|| p->y < 0 || p->y >= max_y)
		return false;
	return true;
}
