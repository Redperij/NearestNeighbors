#include "NearestNeighbors.h"
#include <cmath>

NearestNeighbors::NearestNeighbors(int image_width, int image_height, Point* points,
	int number_of_points) : _image_width(image_width), _image_height(image_height),
	_number_of_points(number_of_points) {

	//Initialise all point neighbors to nullptrs.
	//Copy pointers to points into the _points vector. (Out of bounds points ignored.)
	for (size_t i = 0; i < _number_of_points; i++) {
		points[i].pnearest_neighbor = nullptr;
		if(this->point_in_bounds(&points[i]))
			this->_points.push_back(&points[i]);
	}
	
	//Map pointers to the nearest neighbors of _points to the corresponding indexes in _pnearest_neighbor.
	//All the heavy lifting is done here.
	this->map_nearest_neighbors();
}

NearestNeighbors::~NearestNeighbors() {}

Point* NearestNeighbors::getNearestPoint(Point* p) {
	return p->pnearest_neighbor;
}

void NearestNeighbors::map_nearest_neighbors() {
	//Limit threads to 8.
	for (size_t i = 0; i < this->_points.size(); i++)
		threads.push_back(std::thread(&NearestNeighbors::find_nearest_neighbor,
			this, this->_points[i], i));

	//Min p for t is 50, Max is 512
	//
	size_t size = this->_points.size();

	//case 1: size / 8 > 50
	//case 2: size / 8 <= 50
	


/*	while (size > 0)
	{
		//code
	}
*/	

	for (auto &thr: threads) {
		thr.join();
	}
}

//Must not modify any shared variables.
void NearestNeighbors::find_nearest_neighbor(Point* p, size_t point_index) {
	//Choose algorithm to follow based on ammount of points.
	this->_points.size() > 30 ? fnn_sector(p, point_index) : fnn_linear(p, point_index);
}

//Must not modify any shared variables.
void NearestNeighbors::fnn_linear(Point* p, size_t point_index) {
	float dist = 0;
	float shortest_dist = (float)_image_height * _image_width;
	float dx = 0;
	float dy = 0;
	for (size_t i = 0; i < this->_points.size(); i++) {
		//Found self.
		if (p == _points[i])
			continue;
			
		//Get distances for y and x coordinates.
		dx = p->x - _points[i]->x;
		dy = p->y - _points[i]->y;
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
			p->pnearest_neighbor = _points[i];
		}
	}
}

//Must not modify any shared variables.
void NearestNeighbors::fnn_sector(Point* p, size_t point_index) {
	float dist = 0;
	//Longest possible distance is the diagonal, (which is smaller than width + height).
	float shortest_dist = (float)_image_height + _image_width;
	bool width_longer = this->_image_width > this->_image_height;
	float dx = 0;
	float dy = 0;
	float step = width_longer ? (float)_image_width / 32 : (float)_image_height / 32;
	float sector_half_width = step;
	std::vector<Point*> points_in_bounds;


	//Worst case is 32 runs for one point. (Isolated point)
	while (sector_half_width < (width_longer ? _image_width : _image_height)) {
		
		for (auto &point : _points) {
			//Found self.
			if (p == point)
				continue;
			if (point_in_bounds(point, p->x - step, p->x + step, p->y - step, p->y + step))
				points_in_bounds.push_back(point);
		}

		//Found valid points. (Nearest is among those)
		if (points_in_bounds.size())
			break;

		sector_half_width += step;
	}

	for (auto &point : points_in_bounds) {
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
			p->pnearest_neighbor = point;
		}
	}
}

bool NearestNeighbors::point_in_bounds(Point* p) {
	if (p->x < 0 || p->x >= this->_image_width
		|| p->y < 0 || p->y >= this->_image_height)
		return false;
	return true;
}

bool NearestNeighbors::point_in_bounds(Point* p, const float min_x, const float max_x,
		const float min_y, const float max_y) {
	if (p->x < min_x || p->x >= max_x
		|| p->y < min_y || p->y >= max_y)
		return false;
	return true;
}