#include "NearestNeighbors.h"
#include <cmath>

NearestNeighbors::NearestNeighbors(int image_width, int image_height, Point* points,
	int number_of_points) : _image_width(image_width), _image_height(image_height),
	_number_of_points(number_of_points), _points_per_thread(50), _sector_algorithm_threshold(30) {

	//Initialise all point neighbors to nullptrs.
	//Copy pointers to points into the _points vector. (Out of bounds points ignored.)
	for (size_t i = 0; i < _number_of_points; i++) {
		points[i].pnearest_neighbor = nullptr;
		if(point_in_bounds(&points[i]))
			this->_points.push_back(&points[i]);
	}
	
	//Map pointers to the nearest neighbors of every point in _points.
	//All the heavy lifting is done here.
	map_nearest_neighbors();
}

NearestNeighbors::~NearestNeighbors() {}

Point* NearestNeighbors::getNearestPoint(Point* p) {
	return p->pnearest_neighbor;
}

void NearestNeighbors::map_nearest_neighbors() {
	size_t supported_threads = std::thread::hardware_concurrency();

	//If we cannot determine ammount of threads on the CPU, it's better not to risk.
	if (!supported_threads) {
		find_set_of_nearest_neighbors(0, this->_points.size());
		return;
	}

	//case 1.1: size / supported_threads > _points_per_thread; size % supported_threads == 0
	//case 1.2: size / supported_threads > _points_per_thread; size % supported_threads != 0
	//case 2: size / supported_threads <= _points_per_thread
	size_t size = this->_points.size();
	size_t i = 0;
	size_t step = size > supported_threads * this->_points_per_thread ?
		(size % supported_threads == 0 ? size / supported_threads + 1 : size / supported_threads) :
		this->_points_per_thread;

	do {
		if (i + step < size) {
			threads.push_back(std::thread(&NearestNeighbors::find_set_of_nearest_neighbors, this, i, i + step));
			i += step;
		}
		//Goes here if there is a leftover of points to check.
		else {
			threads.push_back(std::thread(&NearestNeighbors::find_set_of_nearest_neighbors, this, i, size));
			i = size;
		}
	} while (i < size);
	
	for (auto &thr: threads) {
		thr.join();
	}
}

//Must not modify any shared variables.
void NearestNeighbors::find_set_of_nearest_neighbors(size_t start, size_t end) {
	for (size_t i = start; i < end; i++)
		find_nearest_neighbor(this->_points[i]);
}

//Must not modify any shared variables.
void NearestNeighbors::find_nearest_neighbor(Point* p) {
	//Choose algorithm to follow based on ammount of points to compute distance to.
	this->_points.size() > this->_sector_algorithm_threshold ? fnn_sector(p) : fnn_linear(p);
}

//Must not modify any shared variables.
void NearestNeighbors::fnn_linear(Point* p) {
	float dist = 0;
	//Longest possible distance is the diagonal, (which is smaller than width + height).
	float shortest_dist = (float)_image_height * _image_width;
	float dx = 0;
	float dy = 0;

	for (auto &point : this->_points) {
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
			p->pnearest_neighbor = point;
		}
	}
}

//Must not modify any shared variables.
//Array of points must not contain the same point as p.
void NearestNeighbors::fnn_linear(Point* p, std::vector<Point*> &points) {
	float dist = 0;
	//Longest possible distance is the diagonal, (which is smaller than width + height).
	float shortest_dist = (float)_image_height * _image_width;
	float dx = 0;
	float dy = 0;

	for (auto &point : points) {
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

//Must not modify any shared variables.
void NearestNeighbors::fnn_sector(Point* p) {
	bool width_longer = this->_image_width > this->_image_height;
	float step = width_longer ? (float)this->_image_width / 32 : (float)this->_image_height / 32;
	float sector_half_width = step;
	std::vector<Point*> points_in_bounds;

	//Worst case is 32 runs for one point. (Isolated point)
	while (sector_half_width < (width_longer ? this->_image_width : this->_image_height)) {
		
		for (auto &point : this->_points) {
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

	fnn_linear(p, points_in_bounds);
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