#ifndef FUNCTIONS_GEOMETRYFUNCTIONS_HPP_
#define FUNCTIONS_GEOMETRYFUNCTIONS_HPP_

#include "AuxiliaryClasses/Board.hpp"
#include "AuxiliaryClasses/Storage.hpp"
#include <opencv2/core/core.hpp>
#include "AuxiliaryClasses/TypeValidator.hpp"
#include "Constants.hpp"

using namespace std;

/**
	\namepace geo
	the namespace geo supports the software with various mathematical functions needed for geometrical calculations
*/
namespace geo
{
	/**
		\brief member inline function

		@tparam T a primitive data type of the type int, float, double, long, long long or long double

		@param boardPoints a constant reference of a std::vector containing cv::Point objects
		@param board a reference to a aux::Board object of T
		@param coordX a constant T type variable containing the x coordinate of the assembly location
		@param coordY a constant T type variable containing the y coordinate of the assembly location
		@param mX a constant T type variable containing the x coordinate of the center of the assembly location
		@param mX a constant T type variable containing the y coordinate of the center of the assembly location
		@param cornerX a constant T type variable containing the x coordinate of the outer corner of the assembly location
		@param cornerY a constant T type variable containing the y coordinate of the outer corner of the assembly location

		Calculates the geometric vectors to the corner points of the assembly location.

		@return a constant std::vector containing the corner points of the assembly location in the order top left, bottom left, bottom right, top right
	*/
	template <typename T>
	inline std::vector<cv::Point> const generateAssemblyLocation(std::vector<cv::Point> const & boardPoints, aux::Board<T> & board, T const coordX, T const coordY, T const mX, T const mY, T const cornerX, T const cornerY)
	{
		cv::Point tlc, blc, brc, trc;

		if(coordY >= board.boardDimensions()[1] / 2)
		{
			tlc = cv::Point(boardPoints[1] +  (coordX + mX + cornerX) / board.boardDimensions()[0] * (boardPoints[2] - boardPoints[1]) + (1 - (coordY + mY + cornerY) / board.boardDimensions()[1]) * (boardPoints[3] - boardPoints[2]));
			blc = cv::Point(boardPoints[1] +  (coordX + mX + cornerX) / board.boardDimensions()[0] * (boardPoints[2] - boardPoints[1]) + (1 - (coordY + mY - cornerY) / board.boardDimensions()[1]) * (boardPoints[3] - boardPoints[2]));
			brc = cv::Point(boardPoints[1] +  (coordX + mX - cornerX) / board.boardDimensions()[0] * (boardPoints[2] - boardPoints[1]) + (1 - (coordY + mY - cornerY) / board.boardDimensions()[1]) * (boardPoints[3] - boardPoints[2]));
			trc = cv::Point(boardPoints[1] +  (coordX + mX - cornerX) / board.boardDimensions()[0] * (boardPoints[2] - boardPoints[1]) + (1 - (coordY + mY + cornerY) / board.boardDimensions()[1]) * (boardPoints[3] - boardPoints[2]));
		}
		else
		{
			tlc = cv::Point(boardPoints[0] +  (coordX + mX - cornerX) / board.boardDimensions()[0] * (boardPoints[3] - boardPoints[0]) + (coordY + mY - cornerY) / board.boardDimensions()[1] * (boardPoints[2] - boardPoints[3]));
			blc = cv::Point(boardPoints[0] +  (coordX + mX - cornerX) / board.boardDimensions()[0] * (boardPoints[3] - boardPoints[0]) + (coordY + mY + cornerY) / board.boardDimensions()[1] * (boardPoints[2] - boardPoints[3]));
			brc = cv::Point(boardPoints[0] +  (coordX + mX + cornerX) / board.boardDimensions()[0] * (boardPoints[3] - boardPoints[0]) + (coordY + mY + cornerY) / board.boardDimensions()[1] * (boardPoints[2] - boardPoints[3]));
			trc = cv::Point(boardPoints[0] +  (coordX + mX + cornerX) / board.boardDimensions()[0] * (boardPoints[3] - boardPoints[0]) + (coordY + mY - cornerY) / board.boardDimensions()[1] * (boardPoints[2] - boardPoints[3]));
		}

		return std::vector<cv::Point> { tlc, blc, brc, trc };
	}

	/**
		\brief member inline function

		@tparam T a primitive data type of the type int, float, double, long, long long or long double

		@param[out] assemblyLocations a reference of a std::vector of std::vectors containing cv::Point objects to be filled with assembly locations as function output
		@param[in] boardPoints a constant reference to a std::vector containing cv::Point objects
		@param[in] board a reference to a aux::Board object
		@param[in] compNum an constant int containing the number of the box the component is stored in

		@see aux::TypeValidator::validateType()
		@see geo::generateAssemblyLocation<T>()

		Fills assemblyLocations data structure with all the geometric vectors to all assembly locations for all components.
	*/
	template <typename T>
	inline void assemblyPointsVector(std::vector<std::vector<cv::Point>> & assemblyLocations, std::vector<cv::Point> const & boardPoints, aux::Board<T> & board, int const compNum)
	{
		T validator;
		aux::TypeValidator::validateType(validator);

		for(int i = 0; i < board.components()[compNum].occurrences(); i++)
		{
			cv::Mat coordinatesMatrix = board.components()[compNum].coordinates();

			T coordX = board.rotated() ? std::abs(board.originXY()[0] - coordinatesMatrix.at<double>(i, 1)) : std::abs(board.originXY()[0] - coordinatesMatrix.at<double>(i, 0));
			T coordY = board.rotated() ? std::abs(board.originXY()[1] - coordinatesMatrix.at<double>(i, 0)) : std::abs(board.originXY()[1] - coordinatesMatrix.at<double>(i, 1));
			T mX = board.rotated() ? coordinatesMatrix.at<double>(i, 3) : coordinatesMatrix.at<double>(i, 2);
			T mY = board.rotated() ? coordinatesMatrix.at<double>(i, 2) : coordinatesMatrix.at<double>(i, 3);
			T cornerX = board.rotated() ? board.components()[compNum].height() / 2 : board.components()[compNum].width() / 2;
			T cornerY = board.rotated() ? board.components()[compNum].width() / 2 : board.components()[compNum].height() / 2;

			assemblyLocations.push_back(geo::generateAssemblyLocation<T>(boardPoints, board, coordX, coordY, mX, mY, cornerX, cornerY));
		}
	}

	/**
		\brief member inline function

		@tparam T a primitive data type of the type int, float, double, long, long long or long double

		@param storage a reference to a aux::Storage object
		@param col a constant int containing the column number
		@param row a constant int containing the row number

		@see aux::TypeValidator::validateType()

		Calculates the geometric vector factors needed to shrink or stretch given segment vectors in order to gain new points

		@return a constant std::vector of T data type containing the geometric vector factors
	*/
	template <typename T>
	inline std::vector<T> const withdrawalGridPointsVectorsFactors(aux::Storage<T> & storage, int const col, int const row)
	{
		T validator;
		aux::TypeValidator::validateType(validator);

		T storageWidth = storage.refCornerDimensions()[0];
		T storageHeight = storage.refCornerDimensions()[1];
		T boxWidth = storage.boxDimensions()[0];
		T boxHeight = storage.boxDimensions()[1];
		T boxOffsetX = storage.boxOffsetXY()[0];
		T boxOffsetY = storage.boxOffsetXY()[1];

		T xFactor = (boxWidth + boxOffsetX / 2) / storageWidth;
		T yFactor = (boxHeight + boxOffsetY / 2) / storageHeight;

		return std::vector<T> {col * xFactor, row * yFactor};
	}

	/**
		\brief member inline function

		@tparam T a primitive data type of the type int, float, double, long, long long or long double

		@param a a constant T data type variable containing the target x coordinate of a geometric vector
		@param bWidth a constant T data type variable containing the x coordinate of a reference vector
		@param b a constant T data type variable containing the target y coordinate of a geometric vector
		@param bHeight a constant T data type variable containing the y coordinate of a reference vector

		@see aux::TypeValidator::validateType()

		Calculates the geometric vector factors needed to shrink or stretch given segment vectors in order to gain new geometric vectors to new points

		@result a constant std::vector of T data type containing the geometric vector factors
	*/
	template <typename T>
	inline std::vector<T> const vectorFactorXY(T const a, T const bWidth, T const b, T const bHeight)
	{
		T validator;
		aux::TypeValidator::validateType(validator);

		return std::vector<T> { a / bWidth, b / bHeight };
	}

	/**
		\brief member inline function

		@tparam T a primitive data type of the type int, float, double, long, long long or long double

		@param[out] vectorFactors a reference to a std::vector of std::vectors of double to be filled with vector factors as function output
		@param[in] refWidth a constant reference to a T data type variable containing the length of reference points in x
		@param[in] refHeight a constant reference to a T data type variable containing the length of reference points in y
		@param[in] distances a constant reference to a cv::Mat object containing the distances from references to target points

		@see aux::TypeValidator::validateType()
		@see geo::vectorFactorXY<T>()
		@see cnst::NUMBER_CORNERS

		Fills the vectorFactors data structure with all the factors needed to calculate the target points
	*/
	template<typename T>
	inline void targetVectorFactors(std::vector<std::vector<double>> & vectorFactors, T const & refWidth, T const & refHeight, cv::Mat const & distances)
	{
		T validator;
		aux::TypeValidator::validateType(validator);

		for(int i = 0; i < cnst::NUMBER_CORNERS; i++)
			vectorFactors.push_back(vectorFactorXY<T>(distances.at<double>(i, 0), refWidth, distances.at<double>(i, 1), refHeight));
	}


	template <typename T> T const cross(cv::Point const &, cv::Point const &);
	template <typename T> T const norm(cv::Point const &);
	template <typename T> T const dot(cv::Point const &, cv::Point const &);
	template <typename T> T const angleDeg(cv::Point const &, cv::Point const &);

	cv::Point const vectorToPoint(std::vector<cv::Point> const &, std::vector<double> const &);
	cv::Point const intersection(cv::Point const &, cv::Point const &, cv::Point const &, cv::Point const &);

	bool const areTargetAnglesSufficient(std::vector<cv::Point> const &);
}

#endif /* FUNCTIONS_GEOMETRYFUNCTIONS_HPP_ */
