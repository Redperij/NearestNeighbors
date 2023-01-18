#include "NearestNeighbors.h"
#include <cmath>

NearestNeighbors::NearestNeighbors(int image_width, int image_height, Point* points,
	int number_of_points) : _image_width(image_width), _image_height(image_height),
	_number_of_points(number_of_points), _points(points) {

	//Initialise array of pointers to the nearest neighbors.
	_pnearest_neighbor = new Point*[_number_of_points];
	for(size_t i = 0; i < _number_of_points; i++)
		_pnearest_neighbor[i] = nullptr;
	
	//Map pointers to the nearest neighbors of _points to the corresponding indexes in _pnearest_neighbor.
	//All the heavy lifting is done here.
	map_nearest_neighbors_();
}

NearestNeighbors::~NearestNeighbors() {
	delete[] _pnearest_neighbor;
}

Point* NearestNeighbors::getNearestPoint(Point* p) {
	for (size_t i = 0; i < _number_of_points; i++)
		if(p->x == _points[i].x && p->y == _points[i].y)
			return _pnearest_neighbor[i];
	return nullptr;
}

void NearestNeighbors::map_nearest_neighbors_() {
	//Naive implementation.
	for (size_t i = 0; i < _number_of_points; i++)
		//Only for points in bounds.
		if(_points[i].x >= 0 && _points[i].x < _image_width
			&& _points[i].y >= 0 && _points[i].y < _image_height)
			find_nearest_neighbor_(&_points[i], i);
	//Do threads.
}

void NearestNeighbors::find_nearest_neighbor_(Point* p, size_t point_index) {
	
	//Naive implementation.
	float dist = 0;
	float shortest_dist = (float)_image_height * _image_width;
	float dx = 0;
	float dy = 0;
	for (size_t i = 0; i < _number_of_points; i++) {
		//Found self.
		if (p->x == _points[i].x && p->y == _points[i].y)
			continue;
		//Point out of bounds.
		if (_points[i].x < 0 || _points[i].x >= _image_width
			|| _points[i].y < 0 || _points[i].y >= _image_height)
			continue;
		
		//Get distances for y and x coordinates.
		dx = p->x - _points[i].x;
		dy = p->y - _points[i].y;
		dx = dx >= 0 ? dx : dx * -1;
		dy = dy >= 0 ? dy : dy * -1; 
		
		//Get actual distance.
		if (dx == 0)
			dist = dy;
		else if (dy == 0)
			dist = dx;
		else
			dist = (float)sqrt(pow(dx, 2) + pow(dy, 2));
		
		//Compare it to the shortest one.
		if (shortest_dist > dist) {
			shortest_dist = dist;
			this->_pnearest_neighbor[point_index] = &_points[i];
		}
	}
	
	//Do circulation.
}