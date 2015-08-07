#ifndef STORAGE_HPP_
#define STORAGE_HPP_

#include <opencv2/opencv.hpp>
#include "TypeValidator.hpp"

/** \namespace
	part of the namespace aux which supports the software with datastructures
*/
namespace aux
{
	/**
		\class Storage
		\brief Storage class serving as a datastructure for easy data handling concerning Storage relevant data.

		@tparam T a primitive data type of the type int, float, double, long, long long or long double

		Usage: This class serves as the central datastructure for all information regarding the storage!
		It allows generic usage with primitive number data types.
		It holds the data.

		@see d_storageMatrix
		@see d_refMidDimensions
		@see d_refCornerDimensions
		@see d_boxOffsetXY
		@see d_boxDimensions
		@see d_distRefsToCorners
	*/
	template<typename T>
	class Storage
	{
		private:

			/**
				a std::vector containing the kind of storage matrix (m x n)
			*/
			std::vector<int> d_storageMatrix;

			/**
				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				a std::vector containing data type T variables representing the refernce stickers dimension based on their center points
			*/
			std::vector<T> d_refMidDimensions;
			/**
				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				a std::vector containing data type T variables representing the refernce stickers dimension based on their outer corner points
			*/
			std::vector<T> d_refCornerDimensions;

			/**
				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				a std::vector containing data type T variables representing the x and y offsets of the boxes to each other
			*/
			std::vector<T> d_boxOffsetXY;

			/**
				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				a std::vector containing data type T variables representing the dimensions of the Box
			*/
			std::vector<T> d_boxDimensions;

			/**
				a cv::Mat object containing the x and y geometric vectors in order to reach the reference sticker corners
			*/
			cv::Mat d_distRefsToCorners;

		public:

			/**
				explicit Constructor

				@tparam T a primitive data type of the type int, float, double, long, long long or long double

				@param m a std::vector containing ints representing the storage matrix dimensions (m x n)
				@param rmd a standard vector containing data type T variables representing the refernce stickers dimension based on their outer corner points
				@param rcd a std::vector containing data type T variables representing the x and y offsets of the boxes to each other
				@param boXY a std::vector containing data type T variables representing the dimensions of the Box
				@param bd a std::vector containing data type T variables representing the dimensions of the Box
				@param n a cv::Mat object containing the x and y geometric vectors in order to reach the reference sticker corners

				Fills the private data members with their respective data.
				Checks if data type T is a primitive number data type
			*/
			explicit Storage(
					std::vector<int> m,
					std::vector<T> rmd,
					std::vector<T> rcd,
					std::vector<T> boXY,
					std::vector<T> bd,
					cv::Mat n)
				:
					d_storageMatrix(m),
					d_refMidDimensions(rmd),
					d_refCornerDimensions(rcd),
					d_boxOffsetXY(boXY),
					d_boxDimensions(bd),
					d_distRefsToCorners(n)
			{
				T validator;
				TypeValidator::validateType(validator);
			}

			/**
				default Constructor
			*/
			Storage() {}

			/**
				default Destructor
			*/
			~Storage() {}

			/**
				\brief public member inline function

				@see d_storageMatrix
				@return a std::vector containing the kind of storage matrix (m x n) as constant reference
			*/
			std::vector<int> inline const & storageMatrix()
			{
				return this->d_storageMatrix;
			}

			/**
				\brief public member inline function

				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				@see d_refMidDimensions
				@return a std::vector containing data type T variables representing the refernce stickers dimension based on their center points
			*/
			std::vector<T> inline const & refMidDimensions()
			{
				return this->d_refMidDimensions;
			}

			/**
				\brief public member inline function

				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				@see d_refCornerDimensions
				@return a std::vector containing data type T variables representing the refernce stickers dimension based on their outer corner points
			*/
			std::vector<T> inline const & refCornerDimensions()
			{
				return this->d_refCornerDimensions;
			}

			/**
				\brief public member inline function

				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				@see d_boxOffsetXY
				@return a std::vector containing data type T variables representing the x and y offsets of the boxes to each other
			*/
			std::vector<T> inline const & boxOffsetXY()
			{
				return this->d_boxOffsetXY;
			}

			/**
				\brief public member inline function

				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				@see d_boxDimensions
				@return a std::vector containing data type T variables representing the dimensions of the Box
			*/
			std::vector<T> inline const & boxDimensions()
			{
				return this->d_boxDimensions;
			}

			/**
				\brief public member inline function

				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				@see d_distRefsToCorners
				@return a cv::Mat object containing the x and y geometric vectors in order to reach the reference sticker corners
			*/
			cv::Mat inline const & distRefsToCorners()
			{
				return this->d_distRefsToCorners;
			}
	};
}

#endif /* STORAGE_HPP_ */
