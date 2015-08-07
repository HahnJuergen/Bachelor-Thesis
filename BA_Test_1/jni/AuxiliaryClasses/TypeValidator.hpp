#ifndef AUXILIARYCLASSES_TYPEVALIDATOR_HPP_
#define AUXILIARYCLASSES_TYPEVALIDATOR_HPP_

/** \namespace
	part of the namespace aux which supports the software with datastructures
*/
namespace aux
{
	/**
		\class TypeValidator
		\brief TypeValidator class serving as a compilation blocker if previously defined T type variables are not numbers

		Usage: This class serves as a compilation blocker if previously defined T type variables are not numbers!
		It offers an empty overloaded static function with the possible number data types.
		If a T defined data type is not among the valid ones compilation is blocked.
	*/
	class TypeValidator
	{
		public:

			/**
				@name overloaded functions
				This functions have no logic implemented. They block code compilation if generic data types are not defined as number data types
			*/
			//@{
			///Overload with an int reference
			static void validateType(int & i) {}

			///Overload with a long reference
			static void validateType(long & l) {}

			///Overload with a double reference
			static void validateType(double & d) {}

			///Overload with a float reference
			static void validateType(float & f) {}

			///Overload with a long double reference
			static void validateType(long double & ld) {}

			///Overload with a long long reference
			static void validateType(long long & ll) {}
			//@}
	};
}

#endif /* AUXILIARYCLASSES_TYPEVALIDATOR_HPP_ */
