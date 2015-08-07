#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

/**
	\namespace

	a collection of predetermined static constants
*/
namespace cnst
{
	static cv::Scalar const RED(255, 0, 0);
	static cv::Scalar const COLOR_RADIUS_BOTTOM(50, 50, 50, 0);
	static cv::Scalar const COLOR_RADIUS_TOP(50, 50, 50, 0);
	static cv::Scalar const BOTTOM_STICKERS_RED(10, 225, 135, 0);
	static cv::Scalar const TOP_STICKERS_BLUE(150, 108, 110, 0);
	static cv::Scalar const HIGHLIGHTING_COLOR(0, 204, 255);
	static cv::Scalar const COLOR_GREEN(0, 255, 0);
	static cv::Scalar const COLOR_RED(255, 0, 0);
	static cv::Scalar const CONTOUR_MULTIPLIER(4, 4);

	static int const MINIMUM_NUMBER_BOXES_IN_ROW = 1;
	static int const MINIMUM_NUMBER_BOXES_IN_COLUMN = 1;
	static int const INVERSION_FACTOR = -1;
	static int const ANTI_ALIASING = CV_AA;
	static int const HIGHLIGHT_LINE_THICKNESS = 1;
	static int const HIGHLIGHT_LINE_TYPE = 8;
	static int const HIGHLIGHT_SHIFT_NONE = 0;
	static int const MAXIMUM_COLOR_CHANNEL_VALUE = 255;
	static int const MINIMUM_COLOR_CHANNEL_VALUE = 0;
	static int const NUMBER_BLUE_CHANNEL = 0;
	static int const NUMBER_RED_CHANNEL = 2;
	static int const CORRECT_NUMBER_DETECTED_RECTANGLES_BOT_TOP = 2;
	static int const MINIMAL_NUMBER_COMBINABLE_RECTANGLES = 2;
	static int const NUMBER_SCALAR_VALUES = 4;
	static int const SUFFICIENT_NUMBER_DETECTED_RECTANGLES = 4;
	static int const MAXIMUM_VALUE_CORRECT_ANGLE = 20;
	static int const NUMBER_CORNERS = 4;

	static double const RIGHT_ANGLE = 90.0;
	static double const ANGLE_DEGREE_MULTIPLIER = 180.0;
	static double const PI = CV_PI;
	static double const WITHDRAWAL_ALPHA = 0.3;
	static double const WITHDRAWAL_BETA = 0.7;
	static double const MINIMAL_CONTOUR_AREA = 0.1;
	static double const CONTOUR_ARCLENGTH_MULTIPLIER = 0.02;
	static double const IMPLEMENTATION_APLHA_1 = 0.6;
	static double const IMPLEMENTATION_APLHA_2 = 0.5;
	static double const IMPLEMENTATION_BETA_1 = 0.4;
	static double const IMPLEMENTATION_BETA_2 = 0.5;
	static double const GAMMA = 0.0;

	static const char * JAVA_STRING_CLASS_IDENTIFIER = "java/lang/String";
}

#endif /* CONSTANTS_HPP_ */
