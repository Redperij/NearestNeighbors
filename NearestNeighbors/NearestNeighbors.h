#ifndef NEARESTNEIGHBORS_H_
#define NEARESTNEIGHBORS_H_

#include <vector>
#include <thread>

struct Point {
	float x;
	float y;
	Point* pnearest_neighbor;
};

class NearestNeighbors
{
public:
	NearestNeighbors(int image_width, int image_height, Point* points,
		int number_of_points);
	~NearestNeighbors();
	Point* getNearestPoint(Point* p);
private:
	int _image_width;
	int _image_height;
	int _number_of_points;
	const int _points_per_thread;
	const int _sector_algorithm_threshold;
	std::vector<Point*> _points; //InBounds points vector
	//std::vector<Point*> _pnearest_neighbor; //find a way to map it via prev vector.
	std::vector<std::thread> threads;

	void map_nearest_neighbors();
	void find_set_of_nearest_neighbors(size_t start, size_t end);
	void find_nearest_neighbor(Point* p);
	void fnn_linear(Point* p);
	void fnn_linear(Point* p, std::vector<Point*> &points);
	void fnn_sector(Point* p);
	bool point_in_bounds(Point* p);
	bool point_in_bounds(Point* p, const float min_x, const float max_x,
		const float min_y, const float max_y);
};

#endif // !NEARESTNEIGHBORS_H_
