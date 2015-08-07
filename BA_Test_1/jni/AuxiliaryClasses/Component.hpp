#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_

#include <opencv2/opencv.hpp>
#include "TypeValidator.hpp"

/** \namespace
	part of the namespace aux which supports the software with datastructures
*/
namespace aux
{
	/**
		\class Component
		\brief Component class serving as a datastructure for easy data handling concerning component relevant data.

		@tparam T a primitive data type of the type int, float, double, long, long long or long double

		Usage: This class serves as the central datastructure for all information regarding a component!
		It allows generic usage with primitive number data types.
		A Component is a part of a Board, so it is recommended to use the same data type T for a Component used for the Board.
		It holds the data.

		@see d_name
		@see d_occurrences
		@see d_coordinates
		@see d_width
		@see d_height
		@see d_polarity
		@see d_boxNum
	*/
	template<typename T>
	class Component
	{
		private:

			/**
				a std::string containing the name of the component
			*/
			std::string d_name;

			/**
				an int containing the number of occurrences of the Component on the Board
			*/
			int d_occurrences;

			/**
				a cv::Mat object containing the coordinates of each occurrence of the Component with offsets
				in x and y needed to calculate the center points
			*/
			cv::Mat d_coordinates;

			/**
				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				T data type containing the x dimension of the Component
			*/
			T d_width;

			/**
				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				T data type containing the y dimension of the Component
			*/
			T d_height;

			/**
				a bool containing the information whether the Component has polarity (true) or not (false)
			*/
			bool d_polarity;

			/**
				an int containing the number of the Box the Component is stored in
			*/
			int d_boxNum;

		public:

			/**
				explicit Constructor

				@tparam T a primitive data type of the type int, float, double, long, long long or long double

				@param n a string containing the name of the board
				@param o an int containing the number of occurrences of the Component
				@param c a cv::Mat object containing the coordinates and center offsets of each occurrence of the Component
				@param w a data type T containing the width of the Component
				@param h a data type T containing the height of the Component
				@param p a bool containing the information about the polarity of the Component
				@param bn an int containing the number of the Box the Component is stored in

				Fills the private data members with their respective data.
				Checks if data type T is a primitive number data type
			*/
			explicit Component(std::string n, int o, cv::Mat c, T w, T h, bool p, int bn)
			:
			d_name(n), d_occurrences(o), d_coordinates(c), d_width(w), d_height(h), d_polarity(p), d_boxNum(bn)
			{
				T validator;
				TypeValidator::validateType(validator);
			}

			/**
				default constructor
			*/
			Component() {}

			/**
				default destructor
			*/
			~Component() {}

			/**
				\brief public member inline function

				@see d_name
				@return a std::sting containing the name of the Component as constant
			*/
			std::string inline name() const
			{
				return this->d_name;
			}

			/**
				\brief public member inline function

				@see d_occurrences
				@return an int containing the number of occurrences of the Component as constant
			*/
			int inline occurrences() const
			{
				return this->d_occurrences;
			}

			/**
				\brief public member inline function

				@see d_coordinates
				@return a cv::Mat object containing the coordinates and center offsets of each occurrence of the Component as constant
			*/
			cv::Mat inline coordinates() const
			{
				return this->d_coordinates;
			}

			/**
				\brief public member inline function

				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				@see d_width
				@return a data type T containing the width of the Component as constant
			*/
			T inline width() const
			{
				return this->d_width;
			}

			/**
				\brief public member inline function

				@tparam T a primitive data type of the type int, float, double, long, long long or long double
				@see d_height
				@return a data type T containing the height of the Component as constant
			*/
			T inline height() const
			{
				return this->d_height;
			}

			/**
				\brief public member inline function

				@see d_polarity
				@return a bool containing the information whether the Component has polarity (true) or not (false) as constant
			*/
			bool inline polarity() const
			{
				return this->d_polarity;
			}

			/**
				\brief public member inline function

				@see d_boxNum
				@return an int containing number of the Box the Component is stored in as constant
			*/
			int inline boxNum() const
			{
				return this->d_boxNum;
			}
	};
}

#endif /* COMPONENT_HPP_ */
