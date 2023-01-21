#include "test.h"
#include "UniqueRngPoint.h"

using namespace std;

int run_tests() {
    Point* points = new Point[4096];
    int tests_failed = 0;

    tests_failed += run_test_with_valid_points(points, 10, 10, 10, "[Simple valid 10x10]: ");
    tests_failed += run_test(points, 4096, 4096, 4096, "[General 4096 4096x4096]: ");
    tests_failed += run_test_with_valid_points(points, 4096, 4096, 4096, "[Valid 4096 4096x4096]: ");
    tests_failed += run_test_with_valid_points(points, 4096, 1, 1, "[Tightly together]: ");
    tests_failed += run_test(points, 4096, 1, 1, "[Lots of invalid]: ");

    delete[] points;
    return 0;
}

int run_test_with_valid_points(Point* points, int size,
    int image_width, int image_height, const char* test_name) {
    
    int res = 0;
    vector<Point*> check_points;
    Point** obj_neighbors = new Point*[size];
    Point** check_neighbors = new Point*[size];
    clock_t time = 0;
    clock_t total_time = 0;
    size_t missmatch_count = 0;

    generate_valid_unique_points(points, size, image_width, image_height);
    
    for (size_t i = 0; i < size; i++)
        check_points.push_back(&points[i]);

    time = clock();
    NearestNeighbors nb(image_width, image_height, points, size);
    time = clock() - time;
    total_time += time;
    cout << test_name << "Object initialised for " << time << " ms." << endl;

    time = clock();
    for (size_t i = 0; i < size; i++)
        obj_neighbors[i] = nb.getNearestPoint(&points[i]);
    time = clock() - time;
    total_time += time;
    cout << test_name << "Object found neighbors for " << time << " ms." << endl;

    time = clock();
    for (size_t i = 0; i < size; i++) {
        check_neighbors[i] = shortest_dist(check_points[i], check_points);
    }
    time = clock() - time;
    cout << test_name << "Check found neighbors for " << time << " ms." << endl;

    for (size_t i = 0; i < size; i++) {
        if (obj_neighbors[i] != check_neighbors[i]) {
            if(obj_neighbors[i] != nullptr && check_neighbors[i] != nullptr) {
                cout << test_name << "MISSMATCH: Point (" << points[i].x << "; " << points[i].y << ") has different neighbors.\n\t\
                    Object neighbor: (" << obj_neighbors[i]->x << "; " << obj_neighbors[i]->y << ")\n\t\
                    Test neighbor  : (" << check_neighbors[i]->x << "; " << check_neighbors[i]->y << ")" << endl;
                    missmatch_count++;
            }
            else if (obj_neighbors[i] != nullptr) {
                cout << test_name << "MISSMATCH: Point (" << points[i].x << "; " << points[i].y << ") has different neighbors.\n\t\
                    Object neighbor: (" << obj_neighbors[i]->x << "; " << obj_neighbors[i]->y << ")\n\t\
                    Test neighbor  : Gave a nullptr" << endl;
                    missmatch_count++;
            }
            else {
                cout << test_name << "MISSMATCH: Point (" << points[i].x << "; " << points[i].y << ") has different neighbors.\n\t\
                    Object neighbor: Gave a nullptr\n\t\
                    Test neighbor  : (" << check_neighbors[i]->x << "; " << check_neighbors[i]->y << ")" << endl;
                    missmatch_count++;
            }
            
        }
    }
    
    if (missmatch_count) {
        res = 1;
        cout << test_name << "Test completed with errors.\nTotal time spent: " << total_time << " ms" << endl;
    }
    else
        cout << test_name << "Test successful.\nTotal time spent: " << total_time << " ms" << endl;

    delete[] obj_neighbors;
    delete[] check_neighbors;

    return res;
}

int run_test(Point* points, int size, int image_width, int image_height, const char* test_name) {
    int res = 0;
    vector<Point*> check_points;
    Point** obj_neighbors = new Point*[size];
    Point** check_neighbors = new Point*[size];
    clock_t time = 0;
    clock_t total_time = 0;
    size_t missmatch_count = 0;

    generate_unique_points(points, size);
    
    for (size_t i = 0; i < size; i++)
        check_points.push_back(&points[i]);

    time = clock();
    NearestNeighbors nb(image_width, image_height, points, size);
    time = clock() - time;
    total_time += time;
    cout << test_name << "Object initialised for " << time << " ms." << endl;

    time = clock();
    for (size_t i = 0; i < size; i++)
        obj_neighbors[i] = nb.getNearestPoint(&points[i]);
    time = clock() - time;
    total_time += time;
    cout << test_name << "Object found neighbors for " << time << " ms." << endl;

    time = clock();
    for (size_t i = 0; i < size; i++) {
        if (point_in_bounds(&points[i], image_width, image_height))
            check_neighbors[i] = shortest_dist(check_points[i], check_points);
        else
            check_neighbors[i] = nullptr;
    }
    time = clock() - time;
    cout << test_name << "Check found neighbors for " << time << " ms." << endl;

    for (size_t i = 0; i < size; i++) {
        if (obj_neighbors[i] != check_neighbors[i]) {
            if(obj_neighbors[i] != nullptr && check_neighbors[i] != nullptr) {
                cout << test_name << "MISSMATCH: Point (" << points[i].x << "; " << points[i].y << ") has different neighbors.\n\t\
                    Object neighbor: (" << obj_neighbors[i]->x << "; " << obj_neighbors[i]->y << ")\n\t\
                    Test neighbor  : (" << check_neighbors[i]->x << "; " << check_neighbors[i]->y << ")" << endl;
                    missmatch_count++;
            }
            else if (obj_neighbors[i] != nullptr) {
                cout << test_name << "MISSMATCH: Point (" << points[i].x << "; " << points[i].y << ") has different neighbors.\n\t\
                    Object neighbor: (" << obj_neighbors[i]->x << "; " << obj_neighbors[i]->y << ")\n\t\
                    Test neighbor  : Gave a nullptr" << endl;
                    missmatch_count++;
            }
            else {
                cout << test_name << "MISSMATCH: Point (" << points[i].x << "; " << points[i].y << ") has different neighbors.\n\t\
                    Object neighbor: Gave a nullptr\n\t\
                    Test neighbor  : (" << check_neighbors[i]->x << "; " << check_neighbors[i]->y << ")" << endl;
                    missmatch_count++;
            }
            
        }
    }
    
    if (missmatch_count) {
        res = 1;
        cout << test_name << "Test completed with errors.\nTotal time spent: " << total_time << " ms" << endl;
    }
    else
        cout << test_name << "Test successful.\nTotal time spent: " << total_time << " ms" << endl;

    delete[] obj_neighbors;
    delete[] check_neighbors;

    return res;
}

int small_test(Point* points) {
    return 0;
}

void generate_valid_unique_points(Point* points, size_t size, int image_width, int image_height) {
    UniqueRngPoint rnd((float)image_width, (float)image_height);
    for (size_t i = 0; i < size; i++)
        rnd(&points[i]);
}

void generate_unique_points(Point* points, size_t size) {
    UniqueRngPoint rnd(4096, 4096);
    for (size_t i = 0; i < size; i++)
        rnd(&points[i]);
}

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

bool point_in_bounds(Point* p, size_t max_x, size_t max_y) {
	if (p->x < 0 || p->x >= max_x
		|| p->y < 0 || p->y >= max_y)
		return false;
	return true;
}
