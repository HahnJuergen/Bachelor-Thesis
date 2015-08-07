#include "Constants.hpp"
#include "Functions/ProcessingFunctions.hpp"
#include "Functions/FileReadingFunctions.hpp"

using namespace std;

/**
	\brief gnrc member function

	@param[out] frame a reference to a cv::Mat object containing the data of a frame to be modified as function output

	Multiplies the red and blue color channels of each pixel with the respective correction factors in order to perform white balancing.

	@see cnst::NUMBER_BLUE_CHANNEL
	@see cnst::NUMBER_RED_CHANNEL
	@see proc::gnrc::blueFactor()
	@see proc::gnrc::redFactor()
*/
void proc::gnrc::correctColors(cv::Mat & frame)
{
	for(int i = 0; i < frame.rows; i++)
	{
		for(int k = 0; k < frame.cols; k++)
		{
			frame.data[frame.step * i + frame.channels() * k + cnst::NUMBER_BLUE_CHANNEL] *= proc::gnrc::blueFactor;
			frame.data[frame.step * i + frame.channels() * k + cnst::NUMBER_RED_CHANNEL] *= proc::gnrc::redFactor;
		}
	}
}

/**
	\brief gnrc member function

	@param[out] contours a reference to a std::vector of std::vectors containing cv::Point objects to be filled with the detected contours as first function output
	@param[out] maxarea a refernce to a double variable containing the value of the highest detected area as second function output

	@param[in] frame a constant reference to a cv::Mat object containing frame data
	@param[in] lb a constant reference to a cv::Scalar object containing the lower bounds of a color values interval
	@param[in] ub a constant reference to a cv::Scalar object containing the upper bounds of a color values interval

	Calcualtes the detected contours based on their maximal area and stores them into their respective variables

	@see proc::gnrc::contours()
	@see proc::gnrc::maxArea()
	@see proc::gnrc::saveContours()
*/
void proc::gnrc::contourVector(std::vector<std::vector<cv::Point>> & contours, double & maxarea, cv::Mat const & frame, cv::Scalar const & lb, cv::Scalar const & ub)
{
	std::vector<std::vector<cv::Point>> tContours;

	proc::gnrc::contours(tContours, frame, lb, ub);
	proc::gnrc::maxArea(maxarea, tContours);
	proc::gnrc::saveContours(contours, tContours, maxarea);
}

/**
	\brief gnrc member function

	@param[out] rects a reference to a std::vector containing cv::Rect objects to be filled with data as first function output
	@param[out] contours a reference to a std::vector of std::vectors containing cv::Point objects representing the detected contours as second function output

	Builds a bounding rectangle for each detected contour and stores it to its given data structure and clears the data structure of the contours

	@see contourRectangle()
*/
void proc::gnrc::rectangleVector(std::vector<cv::Rect> & rects, std::vector<std::vector<cv::Point>> & contours)
{
	for(std::vector<std::vector<cv::Point>>::iterator it = contours.begin(); it != contours.end(); ++it)
	{
		cv::Rect rect;

		gnrc::contourRectangle(rect, *it);

		rects.push_back(rect);
	}

	contours.clear();
}

/**
	\brief gnrc member function

	@param[out] points a reference to a std::vector containing cv::Point objects to be filled with data as function output
	@param[in] rects a reference to a std::vector containing cv::Rects objects representing the detected areas of a pair of reference stickers
	@param[in] refRects a reference to a std::vector containing cv::Rects objects representing the detected areas of another pair of reference stickers

	Approximates the center point of each reference sticker based on the received rectangle pairs

	@see cnst::CORRECT_NUMBER_DETECTED_RECTANGLES_BOT_TOP
*/
void proc::gnrc::centerPointsVector(std::vector<cv::Point> & points, std::vector<cv::Rect> & rects, std::vector<cv::Rect> & refRects)
{
	if(rects.size() == cnst::CORRECT_NUMBER_DETECTED_RECTANGLES_BOT_TOP && refRects.size() == cnst::CORRECT_NUMBER_DETECTED_RECTANGLES_BOT_TOP)
	{
		vector<cv::Point> refPoints;

		for(vector<cv::Rect>::iterator it = rects.begin(); it != rects.end(); ++it)
			points.push_back(cv::Point(it->x + it->width / 2, it->y + it->height / 2));

		for(vector<cv::Rect>::iterator it = refRects.begin(); it != refRects.end(); ++it)
			refPoints.push_back(cv::Point(it->x + it->width / 2, it->y + it->height / 2));

		if(refPoints[0].x > refPoints[1].x) std::swap(refPoints[0], refPoints[1]);
		if(points[0].x > points[1].x) std::swap(points[0], points[1]);

		points.insert(points.begin(), refPoints[0]);
		points.push_back(refPoints[1]);
	}
}

/**
	\brief asmb member function

	@param[out] targetPoints a reference to a std::vector containing cv::Point objects to be filled with retrieved data as function output
	@param[in] points a constant reference to a std::vector containing cv::Point objects used as geometric vectors to calculate the target points

	Calculates the target corner points of the observed real object via the geometric vectors of the reference stickers and determined shrinking or stretching vectors and stores them to the data structure.

	@see proc::asmb::targetPointsVectors
	@see geo::vectorToPoint()
	@see cnst::NUMBER_CORNERS
*/
void proc::asmb::targetPointsVector(std::vector<cv::Point> & targetPoints, std::vector<cv::Point> const & points)
{
	if(points.size() == cnst::NUMBER_CORNERS && points[0].y < points[1].y)
	{
		cv::Point topLeftTargetPoint = geo::vectorToPoint(vector<cv::Point> {points[0], points[1], points[3]}, proc::asmb::targetPointsVectors[0]);
		cv::Point bottomLeftTargetPoint = geo::vectorToPoint(vector<cv::Point> {points[1], points[0], points[2]}, proc::asmb::targetPointsVectors[1]);
		cv::Point bottomRightTargetPoint = geo::vectorToPoint(vector<cv::Point> {points[2], points[3], points[1]}, proc::asmb::targetPointsVectors[2]);
		cv::Point topRightTargetPoint = geo::vectorToPoint(vector<cv::Point> {points[3], points[2], points[0]}, proc::asmb::targetPointsVectors[3]);

		targetPoints.push_back(topLeftTargetPoint);
		targetPoints.push_back(bottomLeftTargetPoint);
		targetPoints.push_back(bottomRightTargetPoint);
		targetPoints.push_back(topRightTargetPoint);
	}
}

/**
	\brief asmb member function

	@param[out] assemblyPlaces a reference to a std::vector of std::vectors containing cv::Point objects to be filled with retrieved data as function output
	@param[in] boardPoints a constant reference to a std::vector containing cv::Point objects depicting the corner points of a board
	@param[in] compNum a constant int variable containing the number of the component

	Calculates the implementation locations of a component based on its number

	@see cnst::NUMBER_CORNERS
	@see proc::asmb::board
	@see geo::assemblyPointsVector<T>()
*/
void proc::asmb::insertionPlacesVector(std::vector<std::vector<cv::Point>> & assemblyPlaces, std::vector<cv::Point> const & boardPoints, int const compNum)
{
	if(boardPoints.size() == cnst::NUMBER_CORNERS)
		geo::assemblyPointsVector<double>(assemblyPlaces, boardPoints, proc::asmb::board, compNum);
}

/**
	\brief asmb member function

	@param[out] frame a reference to a cv::Mat object containing frame data to be modified as function output
	@param[in] points a constant reference to a std::vector containing cv::Point objects depicting the corner points of the board
	@param[in] locations a constant reference to a std::vector of std::vectors containing cv::Point objects depicting the implementation locations

	Displays all the calculated data

	@see cnst::NUMBER_CORNERS
	@see geo::areTargetAnglesSufficient()
	@see cnst::COLOR_RED
	@see cnst::HIGHLIGHTING_COLOR
	@see cnst::HIGHLIGHT_LINE_THICKNESS
	@see cnst::HIGHLIGHT_SHIFT_NONE
	@see cnst::IMPLEMENTATION_APLHA_1
	@see cnst::IMPLEMENTATION_APLHA_2
	@see cnst::IMPLEMENTATION_BETA_1
	@see cnst::IMPLEMENTATION_BETA_2
	@see cnst::GAMMA
	@see cnst::ANTI_ALIASING
*/
void proc::asmb::displayImplementation(cv::Mat & frame, std::vector<cv::Point> const & points, std::vector<std::vector<cv::Point>> & locations)
{
	if(points.size() == cnst::NUMBER_CORNERS)
	{
		if(geo::areTargetAnglesSufficient(points))
		{
			cv::Mat mat1, mat2;
			frame.copyTo(mat1);
			frame.copyTo(mat2);

			cv::fillConvexPoly(mat1, points, cnst::COLOR_RED);

			for(int i = 0; i < locations.size(); i++)
				cv::fillConvexPoly(mat2, locations[i], cnst::HIGHLIGHTING_COLOR, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::HIGHLIGHT_SHIFT_NONE);


			cv::addWeighted(frame, cnst::IMPLEMENTATION_APLHA_1, mat1, cnst::IMPLEMENTATION_BETA_1, cnst::GAMMA, frame);
			cv::addWeighted(frame, cnst::IMPLEMENTATION_APLHA_2, mat2, cnst::IMPLEMENTATION_BETA_2, cnst::GAMMA, frame);

			cv::line(frame, points[0], points[1], cnst::COLOR_RED, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);
			cv::line(frame, points[0], points[3], cnst::COLOR_RED, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);
			cv::line(frame, points[2], points[1], cnst::COLOR_RED, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);
			cv::line(frame, points[2], points[3], cnst::COLOR_RED, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);

			for(int i = 0; i < locations.size(); i++)
			{
				cv::line(frame, locations[i][0], locations[i][1], cnst::HIGHLIGHTING_COLOR, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);
				cv::line(frame, locations[i][0], locations[i][3], cnst::HIGHLIGHTING_COLOR, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);
				cv::line(frame, locations[i][2], locations[i][1], cnst::HIGHLIGHTING_COLOR, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);
				cv::line(frame, locations[i][2], locations[i][3], cnst::HIGHLIGHTING_COLOR, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);
			}
		}
	}
}

/**
	\brief UNUSED gnrc member function

	@param[out] rects a std::vector containing cv::Rects objects depicting created rectangles at first which is then cleared in filled with retrieved data as function output

	Combines multiple bounding rectangles to one (UNUSED / DEPRECATED)

	@see cnst::MINIMAL_NUMBER_COMBINABLE_RECTANGLES
*/
void proc::gnrc::combineMultipleRectangles(std::vector<cv::Rect> & rects)
{
	if(rects.size() > cnst::MINIMAL_NUMBER_COMBINABLE_RECTANGLES)
	{
		int xMin, xMax, yMin, yMax;

		vector<cv::Rect>::iterator it = rects.begin();

		xMin = it->x;
		xMax = it->x + it->width;
		yMin = it->y;
		yMax = it->y + it->height;

		for(; it != rects.end(); ++it)
		{
			if(it->x < xMin) xMin = it->x;
			if(it->x > xMax) xMin = it->x + it->width;
			if(it->y < yMin) yMin = it->y;
			if(it->y < yMax) yMax = it->y + it->height;
		}

		rects.clear();
		rects.push_back(cv::Rect(xMin, yMin, xMax - xMin, yMax - yMin));
	}
}

/**
	\brief gnrc member function

	@param[out] strings a std::vector of std::vectors containing std::string objects to be filled with retrieved data as function output

	@param[in] mat a constant cv::Mat object containing frame data
	@param[in] ccvWidth a constant reference to an int containing the width of the camera
	@param[in] ccvHeight a constant reference to an int containing the height of the camera
	@param[in] srX a constant reference to an int containing the x coordinate a scan rectangle
	@param[in] srY a constant reference to an int containing the y coordinate a scan rectangle
	@param[in] srWidth a constant reference to an int containing the width of a scan rectangle
	@param[in] srHeight a constant reference to an int containing the height of a scan rectangle
	@param[in] configFilePath a constant reference to an std::string containing the path to a configuration file

	Sets the color correction factors as a mean of white balancing and retrieves data from a configuration file needed for calculation and displaying of data

	@see cnst::NUMBER_SCALAR_VALUES
	@see proc::gnrc::blueFactor
	@see proc::gnrc::redFactor
	@see readf::readFile<T1, T2>
	@see proc::asmb::board
	@see proc::wdr::storage
	@see geo::targetVectorFactors<T>()
	@see proc::asmb::targetPointsVectors
	@see proc::wdr::targetPointsVectors
	@see cnst::INVERSION_FACTOR
*/
void proc::gnrc::calibrate(std::vector<std::vector<std::string>> & strings, cv::Mat const & mat, int const & ccvWidth, int const & ccvHeight, int const & srX, int const & srY, int const & srWidth, int const & srHeight, std::string const & configFilePath)
{
	int xOffset = (ccvWidth - mat.cols) / 2;
	int yOffset = (ccvHeight - mat.rows) / 2;
	int x = srX - xOffset;
	int y = srY - yOffset;

	cv::Rect rect(x, y, srWidth, srHeight);
	cv::Mat regionRGBA(mat, rect);
	cv::Scalar rgb = cv::sum(regionRGBA);

	for(int i = 0; i < cnst::NUMBER_SCALAR_VALUES; i++) rgb.val[i] /= (rect.width * rect.height);

	proc::gnrc::blueFactor = rgb.val[1] / rgb.val[2];
	proc::gnrc::redFactor = rgb.val[1] / rgb.val[0];

	readf::readFile<double, double>(proc::asmb::board, proc::wdr::storage, configFilePath);

	geo::targetVectorFactors<double>(proc::asmb::targetPointsVectors, asmb::board.refDimensions()[0], asmb::board.refDimensions()[1], proc::asmb::board.distRefToCorners());
	geo::targetVectorFactors<double>(proc::wdr::targetPointsVectors, wdr::storage.refMidDimensions()[0], wdr::storage.refMidDimensions()[1], proc::wdr::storage.distRefsToCorners());

	for(int i = 0; i < proc::wdr::targetPointsVectors.size(); i++)
		for(int k = 0; k < proc::wdr::targetPointsVectors[i].size(); k++)
			 proc::wdr::targetPointsVectors[i][k] *= cnst::INVERSION_FACTOR;

	strings.push_back(vector<string> {proc::asmb::board.name()});

	vector<string> temp, temp2, temp3;

	for(int i = 0; i < proc::asmb::board.components().size(); i++)
	{
		int num;
		string str;

		temp.push_back(proc::asmb::board.components()[i].name());

		num = proc::asmb::board.components()[i].occurrences();
		str = static_cast<ostringstream *>(& (ostringstream() << num))->str();
		temp2.push_back(str);

		num = proc::asmb::board.components()[i].polarity();
		str = static_cast<ostringstream *>(& (ostringstream() << num))->str();
		temp3.push_back(str);
	}

	strings.push_back(temp);
	strings.push_back(temp2);
	strings.push_back(temp3);

	regionRGBA.release();
}

/**
	\brief UNUSED gnrc member function

	@param[out] buf a double array with the size 5
	@param[in] frame a constant reference to a cv::Mat object containing frame data
	@param[in] rect a constant reference to a cv::Rect object depicting the scan area

	Calculates the HSV color space values and the correlated color temperature and stores them to the data structure buf

	@see proc::gnrc::correlatedColorTemperature()
	@see cnst::NUMBER_SCALAR_VALUES
*/
void proc::gnrc::measuredImageValues(double buf [5], cv::Mat const & frame, cv::Rect const & rect)
{
	int cct = proc::gnrc::correlatedColorTemperature(frame);

	cv::Mat regionRGBA(frame, rect);
	cv::Mat regionHSV;

	cv::cvtColor(regionRGBA, regionHSV, cv::COLOR_RGB2HSV_FULL);

	cv::Scalar colorHSV = cv::sum(regionHSV);
	cv::Scalar colorRGB = cv::sum(regionRGBA);

	for(int i = 0; i < cnst::NUMBER_SCALAR_VALUES; i++)
	{
		colorHSV.val[i] /= (rect.width * rect.height);
		colorRGB.val[i] /= (rect.width * rect.height);
	}

	regionRGBA.release();
	regionHSV.release();

	for(int i = 0; i < cnst::NUMBER_SCALAR_VALUES; i++) buf[i] = colorHSV.val[i];

	buf[4] = (double) cct;
}

/**
	\brief a gnrc member function

	@param[out] contours a reference to a std::vector of std::vectors containing cv::Point objects to be filled with retrieved data as function output

	@param[in] frame a constant reference to a cv::Mat object containing frame data
	@param[in] lb a constant reference to a cv::Scalar object depicting the lower bounds of a color interval
	@param[in] ub a constant reference to a cv::Scalar object depicting the upper bounds of a color interval

	Detects and stores the contours to the data structure based on a color interval bounded by lower and upper values
*/
void proc::gnrc::contours(std::vector<std::vector<cv::Point>> & contours, cv::Mat const & frame, cv::Scalar const & lb, cv::Scalar const & ub)
{
	cv::Mat mPyrDownMat, mHsvMat, mMask, mDilatedMask;

	std::vector<cv::Vec4i> mHierarchy;

	cv::pyrDown(frame, mPyrDownMat);
	cv::pyrDown(mPyrDownMat, mPyrDownMat);
	cv::cvtColor(mPyrDownMat, mHsvMat, cv::COLOR_RGB2HSV_FULL);
	cv::inRange(mHsvMat, lb, ub, mMask);
	cv::dilate(mMask, mDilatedMask, cv::Mat());
	cv::findContours(mDilatedMask, contours, mHierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	mPyrDownMat.release(); mHsvMat.release(); mMask.release(); mDilatedMask.release();
}

/**
	\brief gnrc member function

	@param[out] maxArea a reference to a double variable containing the value of the biggest detected contour area

	@param[in] contours a reference to a std::vector of std::vectors containing cv::Point objects depicting the detected contours

	Calculates the biggest contour area out of all contour areas
*/
void proc::gnrc::maxArea(double & maxArea, std::vector<std::vector<cv::Point>> & contours)
{
	for(std::vector<std::vector<cv::Point> >::iterator it = contours.begin(); it != contours.end(); ++it)
	{
		double area = cv::contourArea(*it);

		if(area > maxArea) maxArea = area;
	}
}

/**
	\brief gnrc member function

	@param[out] outContours a reference to a std::vector of std::vectors containing cv::Point objects depicting contours to be filled with retrieved data as function output

	@param[in] contours a reference to a std::vector of std::vectors containing cv::Point objects depicting detected contours
	@param[in] maxArea a constant double variable containing the value of the maximal contour area

	Modifies a contour with a multiplying cv::Scalar object if the product of a minimum value and the maxArea variable is less than the contour area of a contour

	@see cnst::MINIMAL_CONTOUR_AREA
	@see cnst::CONTOUR_MULTIPLIER
*/
void proc::gnrc::saveContours(std::vector<std::vector<cv::Point>> & outContours, std::vector<std::vector<cv::Point>> & contours, double const maxArea)
{
	outContours.clear();

	for(std::vector<std::vector<cv::Point> >::iterator it = contours.begin(); it != contours.end(); ++it)
	{
		if(cv::contourArea(*it) > cnst::MINIMAL_CONTOUR_AREA * maxArea)
		{
			cv::multiply(*it, cnst::CONTOUR_MULTIPLIER, *it);

			outContours.push_back(*it);
		}
	}
}

/**
	\brief gnrc member function

	@param[out] rect a reference to a cv::Rect object to be filled with the retrieved data  as function output
	@param[in] contour a constant reference to a std::vector containing cv::Point objects depicting detected contours

	Calculates a bounding rectangle for a given contour

	@see cnst::CONTOUR_ARCLENGTH_MULTIPLIER
*/
void proc::gnrc::contourRectangle(cv::Rect & rect, std::vector<cv::Point> const & contour)
{
	std::vector<cv::Point2f> contour2f, approxCurve;
	std::vector<cv::Point> points;

	cv::Mat(contour).convertTo(contour2f, cv::Mat(contour2f).type());

	double approxDistance = cv::arcLength(contour2f, true) * cnst::CONTOUR_ARCLENGTH_MULTIPLIER;

	cv::approxPolyDP(contour2f, approxCurve, approxDistance, true);
	cv::Mat(approxCurve).convertTo(points, cv::Mat(points).type());

	rect = cv::boundingRect(points);
}

/**
	 \brief UNUSED gnrc member function

	 @param frame a cv::Mat object holding camera frame data

	 Calculates the correlated color temperature of the whole frame

	 @return an int depicting the color temperature in Kelvin
*/
int proc::gnrc::correlatedColorTemperature(cv::Mat const & frame)
{
	cv::Mat regionXYZ;

	cv::cvtColor(frame, regionXYZ, cv::COLOR_RGB2XYZ);

	cv::Scalar colorXYZ = cv::sum(regionXYZ);

	for(int i = 0; i < cnst::NUMBER_SCALAR_VALUES; i++) colorXYZ /= (regionXYZ.cols * regionXYZ.rows);

	double x = colorXYZ.val[0] / (colorXYZ.val[0] + colorXYZ.val[1] + colorXYZ.val[2]);
	double y = colorXYZ.val[1] / (colorXYZ.val[0] + colorXYZ.val[1] + colorXYZ.val[2]);
	double n = (x - 0.3320) / (0.1858 - y);

	regionXYZ.release();

	return (int) ((499 * n * n * n) + (3525 * n * n) + (6823.3 *n) + 5520.33);
}

/**
	\brief wdr member function

	@param[out] corners a reference to a std::vector containing cv::Point objects to be filled with retrieved data as function output
	@param[in] points a constant reference to a std::vector containing cv::Point objects used as geometric vectors to calculate the target points

	Calculates the target corner points of the observed real object via the geometric vectors of the reference stickers and determined shrinking or stretching vectors and stores them to the data structure.

	@see cnst::NUMBER_CORNERS
	@see geo::vectorToPoint()
	@see proc::wdr::targetPointsVectors
*/
void proc::wdr::cornerPointsVector(std::vector<cv::Point> & corners, std::vector<cv::Point> const & points)
{
	if(points.size() == cnst::NUMBER_CORNERS)
	{
		corners.push_back(geo::vectorToPoint(vector<cv::Point> {points[0], points[1], points[3]}, proc::wdr::targetPointsVectors[0]));
		corners.push_back(geo::vectorToPoint(vector<cv::Point> {points[1], points[0], points[2]}, proc::wdr::targetPointsVectors[1]));
		corners.push_back(geo::vectorToPoint(vector<cv::Point> {points[2], points[3], points[1]}, proc::wdr::targetPointsVectors[2]));
		corners.push_back(geo::vectorToPoint(vector<cv::Point> {points[3], points[2], points[0]}, proc::wdr::targetPointsVectors[3]));
	}
}

/**
	\brief wdr member function

	@param[out] tp a reference to a std::vector containing cv::Point objects to be filled with retrieved data as first function output
	@param[out] mp a reference to a std::vector of std::vectors containing cv::Point objects to be filled with retrieved data as second function output
	@param[out] bp a reference to a std::vector containing cv::Point objects to be filled with retrieved data as third function output

	@param[in] cp a constant reference to a std::vector containing cv::Point objects depicting the corner points of an observed real object

	Calculates a grid overlay for the observed real storage matrix

	@see cnst::NUMBER_CORNERS
	@see proc::wdr::edgePointsVectors()
	@see proc::wdr::storage
	@see proc::wdr::middlePointsVectorsVectors()
*/
void proc::wdr::gridPointsVectors(std::vector<cv::Point> & tp, std::vector<std::vector<cv::Point>> & mp, std::vector<cv::Point> & bp, std::vector<cv::Point> const & cp)
{
	if(cp.size() == cnst::NUMBER_CORNERS)
	{
		proc::wdr::edgePointsVectors(tp, bp, cp, proc::wdr::storage.storageMatrix()[1]);
		proc::wdr::middlePointsVectorsVectors(mp, tp, bp, cp, proc::wdr::storage.storageMatrix()[0], proc::wdr::storage.storageMatrix()[1]);
	}
}

/**
	\brief wdr member function

	@param[out] tp a reference to a std::vector containing cv::Point objects to be filled with retrieved data as first function output
	@param[out] bp a reference to a std::vector containing cv::Point objects to be filled with retrieved data as second function output

	@param[in] cp a constant reference to a std::vector containing cv::Point objects depicting the corner points of an observed real object
	@param[in] c as a constant int variable depicting the number of columns

	Calculates the edge points (the upper and lower edge points) of the grid

	@see geo::vectorToPoint()
	@see geo::withdrawalGridPointsVectorsFactors<T>()
	@see proc::wdr::storage
*/
void proc::wdr::edgePointsVectors(std::vector<cv::Point> & tp, std::vector<cv::Point> & bp, std::vector<cv::Point> const & cp, int const c)
{
	cv::Point p, q;

	tp.push_back(cp[0]);
	bp.push_back(cp[1]);

	for(int i = 1; i < c; i++)
	{
		p = geo::vectorToPoint(vector<cv::Point> {cp[0], cp[1], cp[3]}, geo::withdrawalGridPointsVectorsFactors<double>(proc::wdr::storage, i, 0));
		q = geo::vectorToPoint(vector<cv::Point> {cp[1], cp[0], cp[2]}, geo::withdrawalGridPointsVectorsFactors<double>(proc::wdr::storage, i, 0));

		tp.push_back(p);
		bp.push_back(q);
	}

	tp.push_back(cp[3]);
	bp.push_back(cp[2]);
}

/**
	\brief wdr member function

	@param[out] mp a reference to a std::vector of std::vectors containing cv::Point objects to be filled with retrieved data as function output
	@param[out] tp a reference to a std::vector containing cv::Point objects to be filled with retrieved data
	@param[out] bp a reference to a std::vector containing cv::Point objects to be filled with retrieved data

	@param[in] cp a constant reference to a std::vector containing cv::Point objects depicting the corner points of an observed real object
	@param[in] r as a constant int variable depicting the number of rows
	@param[in] c as a constant int variable depicting the number of columns

	Calculates the center points of the grid

	@see geo::vectorToPoint()
	@see geo::withdrawalGridPointsVectorsFactors<T>()
	@see proc::wdr::storage
	@see geo::intersection()
*/
void proc::wdr::middlePointsVectorsVectors(std::vector<std::vector<cv::Point>> & mp, std::vector<cv::Point> & tp, std::vector<cv::Point> & bp, std::vector<cv::Point> const & cp, int const r, int const c)
{
	for(int i = 1; i < r; i++)
	{
		cv::Point u, v;
		vector<cv::Point> me;

		u = geo::vectorToPoint(vector<cv::Point> {cp[1], cp[0], cp[2]}, geo::withdrawalGridPointsVectorsFactors<double>(proc::wdr::storage, 0, i));
		v = geo::vectorToPoint(vector<cv::Point> {cp[2], cp[3], cp[1]}, geo::withdrawalGridPointsVectorsFactors<double>(proc::wdr::storage, 0, i));

		me.push_back(u);

		for(int k = 1; k < c; k++)
			me.push_back(geo::intersection(tp[k], bp[k], u, v));

		me.push_back(v);

		mp.insert(mp.begin(), me);
	}
}

/**
	\brief wdr member function

	@param[out] b a reference to a std::vector containing aux::Box objects to be filled with retrieved data as function output

	@param[in] tp a reference to a std::vector containing cv::Point objects depicting the top edge points
	@param[in] mp a reference to a std::vector of std::vectors containing cv::Point objects depicting the middle points
	@param[in] bp a reference to a std::vector containing cv::Point objects depicting the bottom edge points
	@param[in] cp a constant reference to a std::vector containing cv::Point objects depicting the corner points of an observed real object

	Calculates all boxes in the storage matrixes and stores them in the given data structure

	@see cnst::NUMBER_CORNERS
	@see proc::wdr::storage
	@see cnst::MINIMUM_NUMBER_BOXES_IN_ROW
	@see aux::Box
	@see cnst::MINIMUM_NUMBER_BOXES_IN_COLUMN

*/
void proc::wdr::boxesVector(std::vector<aux::Box> & b, std::vector<cv::Point> & tp, std::vector<std::vector<cv::Point>> & mp, std::vector<cv::Point> & bp, std::vector<cv::Point> const & cp)
{
	if(cp.size() == cnst::NUMBER_CORNERS)
	{
		int const r = proc::wdr::storage.storageMatrix()[0];
		int const c = proc::wdr::storage.storageMatrix()[1];

		if(r > cnst::MINIMUM_NUMBER_BOXES_IN_ROW)
		{
			for(int i = 1; i < r; i++)
			{
				if(i == 1)
					for(int k = 1; k <= c; k++)
						b.push_back(aux::Box(tp[k - 1], mp[i - 1][k - 1], mp[i - 1][k], tp[k]));

				if(i > 1)
					for(int k = 1; k <= c; k++)
						b.push_back(aux::Box(mp[i - 2][k - 1], mp[i - 1][k - 1], mp[i - 1][k], mp[i - 2][k]));

				if(r - i == 1)
					for(int k = 1; k <= c; k++)
						b.push_back(aux::Box(mp[i - 1][k - 1], bp[k - 1], bp[k], mp[i - 1][k]));
			}
		}
		else if(c > cnst::MINIMUM_NUMBER_BOXES_IN_COLUMN)
		{
			for(int i = 1; i <= c; i++)
				b.push_back(aux::Box(tp[i - 1], bp[i - 1], bp[i], tp[i]));
		}
	}
}

/**
	\brief wdr member function

	@param[out] frame a reference to a cv::Mat object containing frame data to be modified as function output
	@param[in] b a reference to a std::vector containing aux::Box objects depicting all boxes of a storage matrix
	@param[in] compNum a constant int variable depicting the number of the box the component is stored in
	@param[in] cp a constant reference to a std::vector containing cv::Point objects depicting the corner points of the storage matrix

	Displays all the calculated data

	@see cnst::NUMBER_CORNERS
	@see geo::areTargetAnglesSufficient()
	@see proc::asmb::board
	@see cnst::COLOR_GREEN
	@see cnst::COLOR_RED
	@see cnst::WITHDRAWAL_ALPHA
	@see cnst::WITHDRAWAL_BETA
	@see cnst::GAMMA
	@see cnst::HIGHLIGHT_LINE_THICKNESS
	@see cnst::ANTI_ALIASING
*/
void proc::wdr::displayWithdrawal(cv::Mat & frame, std::vector<aux::Box> & b, int const compNum, std::vector<cv::Point> const & cp)
{
	if(cp.size() == cnst::NUMBER_CORNERS)
	{
		if(geo::areTargetAnglesSufficient(cp))
		{
			cv::Mat mat;
			frame.copyTo(mat);

			int retrievalIndex = proc::asmb::board.components()[compNum].boxNum() - 1;

			for(int i = 0; i < b.size(); i++)
			{
				if(i == retrievalIndex)
				{
					cv::fillConvexPoly(frame, b[i].corners(), cnst::COLOR_GREEN);
				}
				else
					cv::fillConvexPoly(frame, b[i].corners(), cnst::COLOR_RED);
			}

			cv::addWeighted(frame, cnst::WITHDRAWAL_ALPHA, mat, cnst::WITHDRAWAL_BETA, cnst::GAMMA, frame);

			cv::line(frame, cp[0], cp[1], cnst::COLOR_RED, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);
			cv::line(frame, cp[0], cp[3], cnst::COLOR_RED, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);
			cv::line(frame, cp[2], cp[1], cnst::COLOR_RED, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);
			cv::line(frame, cp[2], cp[3], cnst::COLOR_RED, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);
			cv::line(frame, b[retrievalIndex].corners()[0], b[retrievalIndex].corners()[1], cnst::COLOR_GREEN, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);
			cv::line(frame, b[retrievalIndex].corners()[0], b[retrievalIndex].corners()[3], cnst::COLOR_GREEN, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);
			cv::line(frame, b[retrievalIndex].corners()[2], b[retrievalIndex].corners()[1], cnst::COLOR_GREEN, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);
			cv::line(frame, b[retrievalIndex].corners()[2], b[retrievalIndex].corners()[3], cnst::COLOR_GREEN, cnst::HIGHLIGHT_LINE_THICKNESS, cnst::ANTI_ALIASING);

			mat.release();
		}
	}
}
