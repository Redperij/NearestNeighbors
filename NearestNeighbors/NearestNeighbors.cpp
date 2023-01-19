#include "NearestNeighbors.h"
#include <cmath>

NearestNeighbors::NearestNeighbors(int image_width, int image_height, Point* points,
	int number_of_points) : _image_width(image_width), _image_height(image_height),
	_number_of_points(number_of_points) {

	//Copy pointers to points into the _points vector
	//Out of bound points ignored.
	for (size_t i = 0; i < _number_of_points; i++)
		if(this->point_in_bounds_(&points[i]))
			this->_points.push_back(&points[i]);
	
	//Initialise vector of pointers to the nearest neighbors.
	//Must be an easier way
	this->_pnearest_neighbor.assign(this->_number_of_points, nullptr);
	
	//Map pointers to the nearest neighbors of _points to the corresponding indexes in _pnearest_neighbor.
	//All the heavy lifting is done here.
	this->map_nearest_neighbors_();
}

NearestNeighbors::~NearestNeighbors() {}

Point* NearestNeighbors::getNearestPoint(Point* p) {
	for (size_t i = 0; i < this->_number_of_points; i++)
		if(p == this->_points[i]) //It must be called in an easier way without looping through an array.
			return this->_pnearest_neighbor[i];
	return nullptr;
}

void NearestNeighbors::map_nearest_neighbors_() {
	for (size_t i = 0; i < this->_number_of_points; i++)
		threads.push_back(std::thread(&NearestNeighbors::find_nearest_neighbor_, this, this->_points[i], i));

	for (auto &thr: threads) {
		thr.join();
	}
}

//Must not modify any shared variables.
void NearestNeighbors::find_nearest_neighbor_(Point* p, size_t point_index) {
	//Algorithm.
	/*
	* We have a point (1, 3). We have plane sizes (1024 x 480).
	* From plane size we get a radius step ()
	*
	*/
/*
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
*/	
}

bool NearestNeighbors::point_in_bounds_(Point* p) {
	if (p->x < 0 || p->x >= this->_image_width
		|| p->y < 0 || p->y >= this->_image_height)
		return false;
	return true;
}