#include "NearestNeighbors.h"
#include <cmath>

/**
 * @brief Construct a new NearestNeighbors::NearestNeighbors object
 * @par Functionality
 * Constructor initialises pointer "pnearest_neighbor" of each point
 * to nullptr and finds the nearest neighbor for each valid point. 
 * 
 * @param image_width valid image width (0 < x <= 4096)
 * @param image_height valid image height (0 < y <= 4096)
 * @param points array of unique points (x,y)
 * @param number_of_points size of an array
 */
NearestNeighbors::NearestNeighbors (int image_width, int image_height,
									Point* points, int number_of_points)
	: _image_width (image_width), _image_height (image_height),
	_number_of_points (number_of_points), _points_per_thread (50)
{

	//Initialise all point neighbors to nullptrs.
	//Copy pointers to points into the _points vector.
	//(Out of bounds points ignored.)
	initialise_neighbors (points);
	
	//Map pointers to the nearest neighbors of every point in _points.
	//All the heavy lifting is done here.
	map_nearest_neighbors ();
}

NearestNeighbors::~NearestNeighbors () {}

/**
 * @brief Give a pnearest_neighbor value of Point
 * 
 * @param p Point to copy a value of pnearest_neighbor from.
 * @return Point* pointer to the nearest neighbor point.
 */
Point*
NearestNeighbors::getNearestPoint (Point* p)
{
	return p->pnearest_neighbor;
}

/**
 * @brief Initialises pnearest_neighbor of points and
 * copies valid points pointers to the _points vector.
 * 
 * @param points array of points to initialise.
 */
void
NearestNeighbors::initialise_neighbors (Point* points)
{
	for (size_t i = 0; i < _number_of_points; i++) {
		points[i].pnearest_neighbor = nullptr;
		if (point_in_bounds (&points[i]))
			this->_points.push_back (&points[i]);
	}
}

/**
 * @brief Sets nearest enighbors of valid points.
 * @par Functionality
 * Determins amount of supported threads and divides
 * all points between threads (never less than _points_per_thread)
 */
void
NearestNeighbors::map_nearest_neighbors ()
{
	size_t supported_threads = std::thread::hardware_concurrency ();

	//If we cannot determine ammount of threads on the CPU,
	//it's better not to risk.
	if (!supported_threads) {
		find_set_of_nearest_neighbors (0, this->_points.size());
		return;
	}

	//case 1.1: size / supported_threads > _points_per_thread;
	//			size % supported_threads == 0
	//case 1.2: size / supported_threads > _points_per_thread;
	//			size % supported_threads != 0
	//case 2: size / supported_threads <= _points_per_thread
	size_t size = this->_points.size ();
	size_t i = 0;
	size_t step
		= size > supported_threads * this->_points_per_thread
		? (size % supported_threads == 0 ? size / supported_threads + 1
										 : size / supported_threads)
		: this->_points_per_thread;

	do {
		if (i + step < size) {
			threads.push_back (
				std::thread (&NearestNeighbors::find_set_of_nearest_neighbors,
							this, i, i + step));
			i += step;
		}
		//Goes here if there is a leftover of points to check.
		else {
			threads.push_back (
				std::thread (&NearestNeighbors::find_set_of_nearest_neighbors,
							this, i, size));
			i = size;
		}
	} while (i < size);
	
	for (auto &thr: threads) {
		thr.join ();
	}
}

/**
 * @brief Finds nearest neighbors for set of points.
 * @par Limitations
 * Must not use any shared variables without mutex.
 * 
 * @param start starting poin index.
 * @param end last point index + 1.
 */
void
NearestNeighbors::find_set_of_nearest_neighbors (size_t start, size_t end)
{
	for (size_t i = start; i < end; i++)
		find_nearest_neighbor (this->_points[i]);
}

/**
 * @brief Determines which algorithm to use to find nearest neighbor of point.
 * @par Limitations
 * Must not use any shared variables without mutex.
 * @par Comments
 * Previously it switched between linear and sector algoritms, but sector algorithm
 * appeared to be too slow and hard to implement, thus it was deleted.
 * 
 * @param p Point to find a nearest neighbor for.
 */
void
NearestNeighbors::find_nearest_neighbor (Point* p)
{
	fnn_linear (p);
}

/**
 * @brief Finds nearest neighbor of point using linear algorithm.
 * @par Functionality
 * Loops through every point in the _points vector excluding itself,
 * determining distance to every point keeping a pointer
 * to the point with the shortest distance.
 * @par Limitations
 * - Must not use much computing power.
 * - Must not modify any shared variables except the pnearest_neighbor
 *   of the point it received.
 * 
 * @param p Point to find a nearest neighbor for. 
 */
void
NearestNeighbors::fnn_linear (Point* p)
{
	float dist, dx, dy;
	//Longest possible distance is the diagonal,
	//(which is smaller than width + height).
	float shortest_dist = (float)_image_height + _image_width;

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
			dist = sqrtf (dx * dx + dy * dy);
		
		//Compare it to the shortest one.
		if (shortest_dist > dist) {
			shortest_dist = dist;
			p->pnearest_neighbor = point;
		}
	}
}

/**
 * @brief Determines whether point is inside the image.
 * 
 * @param p Point to check.
 * @return true Point is inside the image.
 * @return false Point is outside the image.
 */
bool
NearestNeighbors::point_in_bounds (Point* p)
{
	return (p->x < 0 || p->x >= this->_image_width
		|| p->y < 0 || p->y >= this->_image_height)
		? false
		: true;
}

/**
 * @brief Determines whether point is inside constraints.
 * 
 * @param p Point to check.
 * @param min_x Lowest width value.
 * @param max_x Highest width value.
 * @param min_y Lowest height value.
 * @param max_y Highest height value.
 * @return true Point is inside constraints.
 * @return false Point is outside constraints.
 */
bool
NearestNeighbors::point_in_bounds (Point* p,
								   const float min_x, const float max_x,
								   const float min_y, const float max_y)
{
	return (p->x < min_x || p->x >= max_x
		|| p->y < min_y || p->y >= max_y)
		? false
		: true;
}
