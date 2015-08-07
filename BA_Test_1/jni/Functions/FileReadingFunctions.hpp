#ifndef FILEREADINGFUNCTIONS_HPP_
#define FILEREADINGFUNCTIONS_HPP_

#include <opencv2/opencv.hpp>
#include "AuxiliaryClasses/Board.hpp"
#include "AuxiliaryClasses/Storage.hpp"
#include <android/log.h>

/** \namespace
	part of the namespace readf which allows the reading of files as a function collection
*/
namespace readf {

	/**
		\brief member inline function

		@tparam T a primitive data type of the type int, float, double, long, long long or long double

		@param[out] components a reference of a std::vector containing aux::Component class objects of the same type T like the aux::Board object containing function output
		@param[in] componentsNode a constant reference of a cv::FileNode object from which data is read iteratively

		Fetches the data of a Component found in the xml file at the given node and stores it as a aux::Component object in components data structure for each aux::Component found in the xml file

		@see aux::Component
	*/
	template <typename T>
	inline void fetchComponentsAttributes(std::vector<aux::Component<T>> & components, cv::FileNode const & componentsNode)
	{
		for(cv::FileNodeIterator it = componentsNode.begin(); it != componentsNode.end(); ++it)
		{
			cv::Mat mat;

			std::string name = (std::string) ((*it) ["name"]);
			int occurrences = (int) ((*it)["occurrences"]);
			(*it) ["coordinates"] >> mat;
			T width = (T) ((*it)["width"]);
			T height = (T) ((*it)["height"]);
			bool polarity = (bool) ((int) ((*it)["polarity"]));
			int boxNum = (int) ((*it)["box"]);

			components.push_back(aux::Component<T>(name, occurrences, mat, width, height, polarity, boxNum));
		}
	}

	/**
		\brief member inline function

		@tparam T1 a primitive data type of the type int, float, double, long, long long or long double
		@tparam T2 a primitive data type of the type int, float, double, long, long long or long double

		@param[out] board a reference of a aux::Board object of T1 to be initialized with retrieved data relevant for Board as first function output
		@param[out] storage a reference of a aux::Storage object of T2 to be initialized with retrieved data relevant for Storage as second function output

		@param[in] filepath a constant reference of a std::string containing the path of the xml file used for data retrieval

		Fetches all the data of the xml file and distributes it to the aux::Board and aux::Storage data structures

		@see fetchComponentsAttributes()
		@see proc::asmb::board
		@see proc::wdr::storage
		@see aux::Board
		@see aux::Component
		@see aux::Storage
	*/
	template <typename T1, typename T2>
	inline void readFile(aux::Board<T1> & board, aux::Storage<T2> & storage, std::string const & filepath)
	{
		std::vector<aux::Component<T1>> components;
		cv::Mat m, n;
		cv::FileStorage fs(filepath, cv::FileStorage::READ);

		fetchComponentsAttributes<T1>(components, fs["components"]);

		std::string name = (std::string) fs["conductor_board"];

		std::vector<T1> originXY = {
				(T1) fs["physicalattributes"]["origin_vector"]["x"],
				(T1) fs["physicalattributes"]["origin_vector"]["y"]
		};

		bool rotation = (bool) ((int) fs["physicalattributes"]["rotation"]);

		std::vector<T1> refDimensions = {
				(T1) fs["physicalattributes"]["referencewidth"],
				(T1) fs["physicalattributes"]["referenceheight"]
		};

		std::vector<T1> boardDimensions = {
				(T1) fs["physicalattributes"]["boardwidth"],
				(T1) fs["physicalattributes"]["boardheight"]
		};

		fs["physicalattributes"]["distRefsToBoardCorners"] >> m;

		board = aux::Board<T1>(name, components, originXY, refDimensions, boardDimensions, rotation, m);


		std::vector<int> storageMatrix = {
				(int) fs["componentstorage"]["rows"],
				(int) fs["componentstorage"]["columns"]
		};

		std::vector<T2> refMidDimensionsStorage = {
				(T2) fs["componentstorage"]["referenceMiddlePointswidth"],
				(T2) fs["componentstorage"]["referenceMiddlePointsheight"]
		};

		std::vector<T2> refCornerDimensionsStorage = {
				(T2) fs["componentstorage"]["referenceCornerPointswidth"],
				(T2) fs["componentstorage"]["referenceCornerPointsheight"]
		};

		std::vector<T2> boxOffsetXY = {
				(T2) fs["componentstorage"]["boxOffsetX"],
				(T2) fs["componentstorage"]["boxOffsetY"]
		};

		std::vector<T2> boxDimensions = {
				(T2) fs["componentstorage"]["boxWidth"],
				(T2) fs["componentstorage"]["boxHeight"]
		};

		fs["componentstorage"]["distRefsToStorageCorners"] >> n;

		storage = aux::Storage<T2>(storageMatrix, refMidDimensionsStorage, refCornerDimensionsStorage, boxOffsetXY, boxDimensions, n);

		fs.release();
	}
}

#endif /* FILEREADINGFUNCTIONS_HPP_ */
