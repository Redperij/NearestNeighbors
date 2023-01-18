#ifndef NEARESTNEIGHBORS_H_
#define NEARESTNEIGHBORS_H_

struct Point {
	float x;
	float y;
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
	Point* _points;
	Point** _pnearest_neighbor;

	void map_nearest_neighbors_();
	void find_nearest_neighbor_(Point* p, size_t point_index);
};

#endif // !NEARESTNEIGHBORS_H_