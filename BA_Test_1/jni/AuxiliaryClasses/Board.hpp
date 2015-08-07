#ifndef BOARD_HPP_
#define BOARD_HPP_

#include "Component.hpp"
#include <opencv2/opencv.hpp>
#include "TypeValidator.hpp"
#include "android/log.h"

/** \namespace
	part of the namespace aux which supports the software with datastructures
*/
namespace aux
{

/** \class Board
	\brief Board class serving as a datastructure for easy data handling concerning board relevant data
	allowing generic use of primitive number data types

	@tparam T a primitive data type of the type int, float, double, long, long long or long double

	Usage: This class serves as the central datastructure for all information regarding the board!
	It allows generic usage with primitive number datatypes.
	It holds the data.

	@see d_name
	@see d_components
	@see d_originXY
	@see d_refDimensions
	@see d_boardDimensions
	@see d_rotated
	@see d_distRefToCorners
*/
	template <typename T>
	class Board
	{
		private:
			/**
				a std::string containing the name of the board
			*/
			std::string d_name;

			/**
				a std::vector containing objects of the Component class containing each
				component of the board
				@tparam T a primitive data type of the type int, float, double, long, long long or long double
			*/
			std::vector<Component<T>> d_components;

			/**
				a std::vector containing primitive number data type variables representing the
				geometric vector of the boards left upper corner to the real origin
				@tparam T a primitive data type of the type int, float, double, long, long long or long double
			*/
			std::vector<T> d_originXY;

			/**
				a std::vector containing primitive number data type variables representing
				the reference sticker dimensions
				@tparam T a primitive data type of the type int, float, double, long, long long or long double
			*/
			std::vector<T> d_refDimensions;

			/**
				a std::vector containing primitive number data type variables representing
				the board dimensions
				@tparam T a primitive data type of the type int, float, double, long, long long or long double
			*/
			std::vector<T> d_boardDimensions;

			/**
			 	a bool variable containing the information whether the real world coordinate system
			 	 is rotaded relative to the camera coordinate system (true) or not (false)
			*/
			bool d_rotated;

			/**
				a cv::Mat object containing the geometric vectors from each reference sticker
				to its respective closest board corner
			*/
			cv::Mat d_distRefToCorners;

		public:

			/**
				explicit Constructor

				@tparam T a primitive data type of the type int, float, double, long, long long or long double

				@param n a string argument containing the name of the board
				@param cs a std::vector containing objects of Component class
				@param o a std::vector containing the geometric vector to the origin
				@param rd a std::vector containing the reference sticker dimensions
				@param bd a std::vector containing the board dimensions
				@param r a bool containing the coordinate system rotation
				@param m a cv::Mat containing the the geometric vectors of the reference stickers to the board corners

				Fills the private data members with their respective data.
				Checks if T is a primitive number data type
			*/
			explicit Board(
					std::string n,
					std::vector<Component<T>> cs,
					std::vector<T> o,
					std::vector<T> rd,
					std::vector<T> bd,
					bool r,
					cv::Mat m)
				:
					d_name(n),
					d_components(cs),
					d_originXY(o),
					d_refDimensions(rd),
					d_boardDimensions(bd),
					d_rotated(r),
					d_distRefToCorners(m)
			{
				T validator;
				TypeValidator::validateType(validator);
			}

			/**
				default constructor
			*/
			Board() {}

			/**
				default destructor
			*/
			~Board() {}

			/**
				\brief public member inline function

				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				@see d_name
				@return a std::string containing the name of the Board as constant reference
			*/
			std::string inline const & name()
			{
				return this->d_name;
			}

			/**
				\brief public member inline function

				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				@see d_components
				@return a std::vector of components as constant reference
			*/
			std::vector<Component<T>> inline const & components()
			{
				return this->d_components;
			}

			/**
			 	\brief public member inline function

			 	@tparam T a primitive data type of the type int, float, double, long, long long or long double
			 	@see d_originXY
			 	@return a std::vector of geometric vectors to the origin as constant reference
			*/
			std::vector<T> inline const & originXY()
			{
				return this->d_originXY;
			}

			/**
			 	\brief public member inline function

			 	@tparam T a primitive data type of the type int, float, double, long, long long or long double
				@see d_refDimensions
				@return a std::vector of the reference sticker dimensions as constant reference
			*/
			std::vector<T> inline const & refDimensions()
			{
				return this->d_refDimensions;
			}

			/**
				\brief public member inline function

				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				@see d_boardDimensions
				@return a std::vector of the board dimensions as constant reference
			*/
			std::vector<T> inline const & boardDimensions()
			{
				return this->d_boardDimensions;
			}

			/**
				\brief public member inline function

				@see d_rotated
				@return a bool containing the value whether the real word coordinate system is rotated relative
				to the camera coordinate system (true) or not (false)
			*/
			bool inline const & rotated()
			{
				return this->d_rotated;
			}

			/**
				\brief public member inline function

				@see d_distRefToCorners
				@return a cv::Mat object containing the geometric vectors from each reference sticker
				to its respective closest board corner
			*/
			cv::Mat inline const & distRefToCorners()
			{
				return this->d_distRefToCorners;
			}
	};
}

#endif /* BOARD_HPP_ */
