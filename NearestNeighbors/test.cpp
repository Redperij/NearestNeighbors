#include "test.h"
#include "UniqueRngPoint.h"

using namespace std;

void run_old_test() {
	Point* points = new Point[4096];
    int size = 10;
    int image_width = 4096;
    int image_height = 4096;
    clock_t time = 0;
    generate_int_points(points, size);

    for (size_t i = 0; i < 2; i++) {
        cout << "************************************************************************************" << endl;
        cout << "Image " << i << ": (" << image_width << "x" << image_height << ") with "
            << size << " points generated." << endl;
        cout << "Image " << i << ": Initialising..." << endl;
        time = clock();
        NearestNeighbors nb(image_width, image_height, points, size);
        time = clock() - time;
        cout << "Image " << i << ": Initialised for " << time << " clock ticks." << endl;

        for (size_t q = 0; q < size; q++) {
            time = clock();
            Point* res = nb.getNearestPoint(&points[q]);
            time = clock() - time;
            if (res != nullptr)
                cout << q << ": Nearest neighbor for point (" << points[q].x << ", " << points[q].y
                    << ") is (" << res->x << ", " << res->y << ") found for " << time << " clock ticks." << endl;
            //else
                //cout << q << ": Point (" << points[q].x << ", " << points[q].y << ") is out of bounds." << endl;
        }

        //image_width = rand() % 4097; //use these for random image sizes.
        //image_height = rand() % 4097;
        size = 4096;
        //size = (long long)rand() % 4096 + 1;
        generate_points(points, size);
    }
    
    delete[] points;
}

void generate_rand_points(Point *points, int &n) {
    n = (long long)rand() % 4096 + 1; //Use this for random number of points.
    for (size_t i = 0; i < n; i++) {
        points[i].x = (float) (rand() % 4096 + (float)(rand() % 10000) / 10000);
        points[i].y = (float) (rand() % 4096 + (float)(rand() % 10000) / 10000);
    }
}

void generate_points(Point* points, size_t size) {
    for (size_t i = 0; i < size; i++) {
        points[i].x = (float) (rand() % 4096 + (float)(rand() % 10000) / 10000);
        points[i].y = (float) (rand() % 4096 + (float)(rand() % 10000) / 10000);
    }
}

void generate_int_points(Point* points, size_t size) {
    for (size_t i = 0; i < size; i++) {
        points[i].x = (float)((int) (rand() % 10));
        points[i].y = (float)((int) (rand() % 10));
    }
}

int run_tests() {
    Point* points = new Point[4096];
    int size = 10;
    int image_width = 4096;
    int image_height = 4096;
    clock_t time = 0;

    generate_valid_unique_points(points, size, image_width, image_height);
    cout << "Generated:" << endl;
    for (size_t i = 0; i < size; i++)
    {
        cout << "(" << points[i].x << ", " << points[i].y << "), ";
    }
    cout << endl;
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
