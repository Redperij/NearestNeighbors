#ifndef NEARESTNEIGHBORS_H_
#define NEARESTNEIGHBORS_H_

#include <vector>
#include <thread>
#include <mutex>

struct Point {
	float x;
	float y;
	Point* pnearest_neighbor;
};

class NearestNeighbors {
public:
	NearestNeighbors (int image_width, int image_height, Point* points,
		int number_of_points);
	~NearestNeighbors ();
	Point* getNearestPoint (Point* p);
private:
	const int _image_width;
	const int _image_height;
	const int _number_of_points;
	const int _points_per_thread;
	const int _sector_algorithm_threshold;
	//Coefficient to use for determining sector and step size.
	float _image_sector_div_coef;
	std::vector<Point*> _points; //InBounds points vector
	std::vector<std::thread> threads;
	std::mutex guard;

	void initialise_neighbors (Point* points);
	void map_nearest_neighbors ();
	void find_set_of_nearest_neighbors (size_t start, size_t end);
	void find_nearest_neighbor (Point* p);
	void fnn_linear (Point* p);
	Point* fnn_sector_linear (Point* p, Point** points, 
							  const size_t size, const float radius);
	void fnn_sector (Point* p);
	bool point_in_bounds (Point* p);
	bool point_in_bounds (Point* p, const float min_x, const float max_x,
						  const float min_y, const float max_y);
};

#endif // !NEARESTNEIGHBORS_H_
