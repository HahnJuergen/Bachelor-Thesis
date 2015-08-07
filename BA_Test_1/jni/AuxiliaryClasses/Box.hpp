#ifndef AUXILIARYCLASSES_BOX_HPP_
#define AUXILIARYCLASSES_BOX_HPP_

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

/** \namespace
	part of the namespace aux which supports the software with datastructures
*/
namespace aux
{
	/**
		\class Box
		\brief Box class serving as a datastructure for easy data handling concerning box relevant data

		Usage: This class serves as the central datastructure for all information regarding a box!
		It holds the data.

		@see d_topLeftCorner
		@see d_bottomLeftCorner
		@see d_bottomRightCorner
		@see d_topRightCorner
	*/
	class Box
	{
		private:
			/**
				a cv::Point object representing the top left corner
			*/
			cv::Point d_topLeftCorner;

			/**
				a cv::Point object representing the bottom left corner
			*/
			cv::Point d_bottomLeftCorner;

			/**
				a cv::Point object representing the bottom right corner
			*/
			cv::Point d_bottomRightCorner;

			/**
				a cv::Point object representing the top right corner
			*/
			cv::Point d_topRightCorner;

		public:

			/**
				explicit constructor

				@param a a constant cv::Point object representing the top left corner
				@param b a constant cv::Point object representing the bottom left corner
				@param c a constant cv::Point object representing the bottom right corner
				@param d a constant cv::Point object representing the top right corner

				Fills the private data members with their respective data
			*/
			explicit Box(cv::Point const a, cv::Point const b,
					cv::Point const c, cv::Point const d)
				:
					d_topLeftCorner(a),	d_bottomLeftCorner(b),
					d_bottomRightCorner(c),	d_topRightCorner(d)
			{}

			/**
				default constructor
			*/
			Box() {}

			/**
				default destructor
			*/
			~Box() {}

			/**
				\brief public member inline function

				@see d_topLeftCorner
				@return a cv::Point object for the top left corner as constant reference
			*/
			cv::Point inline const & topLeftCorner()
			{
				return this->d_topLeftCorner;
			}

			/**
				\brief public member inline function

				@see d_bottomLeftCorner
				@return a cv::Point object for the bottom left corner as constant reference
			*/
			cv::Point inline const & bottomLeftCorner()
			{
				return this->d_bottomLeftCorner;
			}

			/**
				\brief public member inline function

				@see d_bottomRightCorner
				@return a cv::Point object for the bottom right corner as constant reference
			*/
			cv::Point inline const & bottomRightCorner()
			{
				return this->d_bottomRightCorner;
			}

			/**
				\brief public member inline function

				@see d_topRightCorner
				@return a cv::Point object for the top right corner as constant reference
			*/
			cv::Point inline const & topRightCorner()
			{
				return this->d_topRightCorner;
			}

			/**
				\brief public member inline function

				@see d_topLeftCorner
				@see d_bottomLeftCorner
				@see d_bottomRightCorner
				@see d_topRightCorner

				@return a std::vector containing cv::Point objects representing the corner points as constant
			*/
			std::vector<cv::Point> inline const corners()
			{
				return std::vector<cv::Point>
				{
					this->d_topLeftCorner, this->d_bottomLeftCorner,
					this->d_bottomRightCorner, this->d_topRightCorner
				};
			}
	};
}

#endif /* AUXILIARYCLASSES_BOX_HPP_ */
