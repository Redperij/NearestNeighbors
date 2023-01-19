#ifndef NEARESTNEIGHBORS_H_
#define NEARESTNEIGHBORS_H_

#include <vector>
#include <thread>

struct Point {
	float x;
	float y;
	Point* np;
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
	std::vector<Point*> _points; //InBounds vector
	//OutOfBounds vector //redundant
	std::vector<Point*> _pnearest_neighbor; //find a way to map it via prev vector.
	std::vector<std::thread> threads;

	void map_nearest_neighbors_();
	void find_nearest_neighbor_(Point* p, size_t point_index);
	bool point_in_bounds_(Point* p);
};

#endif // !NEARESTNEIGHBORS_H_