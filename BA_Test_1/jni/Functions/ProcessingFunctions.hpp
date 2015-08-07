#ifndef PROCESSINGFUNCTIONS_HPP_
#define PROCESSINGFUNCTIONS_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "AuxiliaryClasses/Board.hpp"
#include "AuxiliaryClasses/Component.hpp"
#include "AuxiliaryClasses/Box.hpp"
#include "Functions/GeometryFunctions.hpp"
#include "AuxiliaryClasses/Storage.hpp"

/**
	\namespace proc

	the namespace proc contains the function collections needed for image processing and result displaying for both the widthdrawal step and implementation step
*/
namespace proc
{

	/**
		\namespace proc::gnrc

		the namespace gnrc contains the generic function collection needed for image processing which is applicable for both the withdrawal step and implementation step
	*/
	namespace gnrc
	{
		/**
		 	\brief blue color channel correction factor

			This factor is used to correct the blue channel of a pixel by multiplying the initial value with this one
		*/
		static double blueFactor;

		/**
			\brief blue color channel correction factor

			This factor is used to correct the red channel of a pixel by multiplying the initial value with this one
		*/
		static double redFactor;

		void correctColors(cv::Mat &);
		void contourVector(std::vector<std::vector<cv::Point>> &, double &, cv::Mat const &, cv::Scalar const &, cv::Scalar const &);
		void rectangleVector(std::vector<cv::Rect> &, std::vector<std::vector<cv::Point>> &);
		void centerPointsVector(std::vector<cv::Point> &, std::vector<cv::Rect> &, std::vector<cv::Rect> &);
		void combineMultipleRectangles(std::vector<cv::Rect> &);
		void calibrate(std::vector<std::vector<std::string>> &, cv::Mat const &, int const &,  int const &,  int const &,  int const &,  int const &,  int const &, std::string const &);
		void measuredImageValues(double [5], cv::Mat const &, cv::Rect const &);
		void contours(std::vector<std::vector<cv::Point>> &, cv::Mat const &, cv::Scalar const &, cv::Scalar const &);
		void maxArea(double &, std::vector<std::vector<cv::Point>> &);
		void saveContours(std::vector<std::vector<cv::Point>> &,  std::vector<std::vector<cv::Point>> &, double const);
		void contourRectangle(cv::Rect &, std::vector<cv::Point> const &);
		int correlatedColorTemperature(cv::Mat const &);
	}

	/**
		\namespace proc::asmb

		the namespace asmb contains the function collection which is specific for the implementation step
	*/
	namespace asmb
	{
		/**
			\brief the shrinking or stretching factors used to calculate the target points needed for assembly
		*/
		static std::vector<std::vector<double>> targetPointsVectors;

		/**
			\brief the aux::Board object used for all calculations based on board specific values
		*/
		static aux::Board<double> board;

		void targetPointsVector(std::vector<cv::Point> &, std::vector<cv::Point> const &);
		void insertionPlacesVector(std::vector<std::vector<cv::Point>> &, std::vector<cv::Point> const &, int const);
		void displayImplementation(cv::Mat &, std::vector<cv::Point> const &, std::vector<std::vector<cv::Point>> &);
	}

	/**
		\namespace proc::wdr

		the namespace wdr contains the function collection which is specific for the withdrawal step
	*/
	namespace wdr
	{
		/**
			\brief the shrinking or stretching factors used to calculate the target points needed for withdrawal
	 	*/
		static std::vector<std::vector<double>> targetPointsVectors;

		/**
			the aux::Storage object used for all calculations based on storage specific values
		*/
		static aux::Storage<double> storage;

		void cornerPointsVector(std::vector<cv::Point> &, std::vector<cv::Point> const &);
		void gridPointsVectors(std::vector<cv::Point> &, std::vector<std::vector<cv::Point>> &, std::vector<cv::Point> &, std::vector<cv::Point> const &);
		void edgePointsVectors(std::vector<cv::Point> &, std::vector<cv::Point> &, std::vector<cv::Point> const &, int const);
		void middlePointsVectorsVectors(std::vector<std::vector<cv::Point>> &, std::vector<cv::Point> &, std::vector<cv::Point> &, std::vector<cv::Point> const &, int const, int const);
		void boxesVector(std::vector<aux::Box> &, std::vector<cv::Point> &, std::vector<std::vector<cv::Point>> &, std::vector<cv::Point> &, std::vector<cv::Point> const &);
		void displayWithdrawal(cv::Mat &, std::vector<aux::Box> &, int const, std::vector<cv::Point> const &);
	}
}

#endif /* PROCESSINGFUNCTIONS_HPP_ */
