#include "Functions/GeometryFunctions.hpp"
#include "Constants.hpp"
using namespace std;

/**
	\brief member function

	@param p a constant reference to a cv::Point object
	@param q a constant reference to a cv::Point object
	@param r a constant reference to a cv::Point object
	@param s a constant reference to a cv::Point object

	@see geo::cross()

	Calculates the intersection point of the line running through p and q and the line running through r and s

	@return a constant cv::Point object containing the intersection point of the line running through p and q and the line running through r and s
*/
cv::Point const geo::intersection(cv::Point const & p, cv::Point const & q, cv::Point const & r, cv::Point const & s)
{
	cv::Point x = r - p;
	cv::Point d1 = q - p;
	cv::Point d2 = s - r;

	double t1 = geo::cross<double>(x, d2) / geo::cross<double>(d1, d2);

	return p + d1 * t1;
}

/**
	\brief member function

	@param points a constant reference to a std::vector containing cv::Point objects representing geometric vectors
	@param factors a constant reference to a std::vector containing double variables representing the factors needed for shrinking or stretching

	Calculates a new point based on the parameter geometric vectors and factors

	@return a constant cv::Point object containing the coordinates of the newly calculated point
*/
cv::Point const geo::vectorToPoint(std::vector<cv::Point> const & points, std::vector<double> const & factors)
{
	return (cv::Point((points[1] - points[0]) * factors[1] + factors[0] * (points[2] - points[0]) + points[0]));
}

/**
	\brief member function

	@tparam T a primitive data type of the type int, float, double, long, long long or long double

	@param a a constant reference to a cv::Point object
	@param b a constant reference to a cv::Point object

	@see aux::TypeValidator::validateType()

	Calculates the cross product of two 2D geometric vectors represented by cv::Point objects

	@return a constant T data type variable containing the cross product value of two 2D geometric vectors
*/
template <typename T>
T const geo::cross(cv::Point const & a, cv::Point const & b)
{
	T validator;
	aux::TypeValidator::validateType(validator);

	return (T) (a.x * b.y - a.y * b.x);
}

/**
	\brief member function

	@tparam T a primitive data type of the type int, float, double, long, long long or long double

	@param a a constant reference to a cv::Point object

	@see aux::TypeValidator::validateType()

	Calculates the norm of a 2D geometric vector represented by a cv::Point

	@return a constant T data type variable containing the geometric vector norm value of a 2D geometric vector
*/
template <typename T>
T const geo::norm(cv::Point const & a)
{
	T validator;
	aux::TypeValidator::validateType(validator);

	return (T) std::sqrt(a.x * a.x + a.y * a.y);
}

/**
	\brief member function

	@tparam T a primitive data type of the type int, float, double, long, long long or long double

	@param a a constant reference to a cv::Point object
	@param b a constant reference to a cv::Point object

	@see aux::TypeValidator::validateType()

	Calculates the dot product of two 2D geometric vectors represented by cv::Point objects

	@return a constant T data type variable containing the dot product value of two 2D geometric vectors
*/
template <typename T>
T const geo::dot(cv::Point const & a, cv::Point const & b)
{
	T validator;
	aux::TypeValidator::validateType(validator);

	return (T) (a.x * b.x + a.y * b.y);
}

/**
	\brief member function

	@tparam T a primitive data type of the type int, float, double, long, long long or long double

	@param a a constant reference to a cv::Point object
	@param b a constant reference to a cv::Point object

	@see aux::TypeValidator::validateType()
	@see geo::norm<T>()
	@see geo::dot<T>()

	Calculates angle in degrees between to 2D geometric vectors represented by cv::Point objects

	@return a constant T data type variable containing the angle in degrees between two 2D geometric vectors
*/
template <typename T>
T const geo::angleDeg(cv::Point const & a, cv::Point const & b)
{
	T validator;
	aux::TypeValidator::validateType(validator);

	return (T) (std::acos(geo::dot<double>(a, b) / (geo::norm<double>(a) * geo::norm<double>(b))) * cnst::ANGLE_DEGREE_MULTIPLIER / cnst::PI);
}

/**
	\brief member function

	@param points a constant reference to a std::vector containing cv::Point objects

	@see cnst::RIGHT_ANGLE
	@see cnst::MAXIMUM_VALUE_CORRECT_ANGLE
	@see geo::angleDeg<T>()

	Checks the corner angles made by geometric vectors represented by cv::Point objects stored in the std::vector points parameter

	@return a constant bool containing true if the average angle in degrees is below the angle threshold, false if not
*/
bool const geo::areTargetAnglesSufficient(std::vector<cv::Point> const & points)
{
	double angleA = geo::angleDeg<double>(points[3] - points[0], points[1] - points[0]);
	double angleB = geo::angleDeg<double>(points[0] - points[1], points[2] - points[1]);
	double angleC = geo::angleDeg<double>(points[1] - points[2], points[3] - points[2]);
	double angleD = geo::angleDeg<double>(points[0] - points[3], points[2] - points[3]);


	double diffA = std::abs(cnst::RIGHT_ANGLE - angleA);
	double diffB = std::abs(cnst::RIGHT_ANGLE - angleB);
	double diffC = std::abs(cnst::RIGHT_ANGLE - angleC);
	double diffD = std::abs(cnst::RIGHT_ANGLE - angleD);

	return (diffA + diffB + diffC + diffD) / points.size() < cnst::MAXIMUM_VALUE_CORRECT_ANGLE;
}
