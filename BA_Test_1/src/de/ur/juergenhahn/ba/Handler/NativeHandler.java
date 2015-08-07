package de.ur.juergenhahn.ba.Handler;

/**
	\class NativeHandler
	
	Calls the Java Native Interface functions written in C++ with the help of the Android Native Development Kit (NDK) where a
	frame is processed based on the called function
*/
public class NativeHandler {
	
	/**
		\brief private native static member function
		
		@param frameAdress a long variable containing the memory address of a Mat object containing frame data
		@param values an array of double variables depicting the coordinates of the scan rectangle
		
		Retrieves the color channel values based on the scan rectangle measurement area and the correlated color temperature.
		
		@see Native Library Documentation for further details.
		@see Java_de_ur_juergenhahn_ba_Handler_NativeHandler_measureImageValuesNative()
	
		@return an array of double variables containing the color channel values and correlated color temperature
	*/
	private native static double[] measureImageValuesNative(long frameAdress, double[] values);

	/**
		\brief private native static member function
	
		@param frameAddress a long variable containing the memory address of a Mat object containing frame data
		@param ccvWidth an int variable containing the width of a custom camera view
		@param ccvHeight an int variable containing the width of a custom camera view
		@param srX an int variable containing the x coordinate of the scan rectangle
		@param srY an int variable containing the y coordinate of the scan rectangle
		@param srWidth an int variable containing the width of the scan rectangle
		@param srHeight an int variable containing the height of the scan rectangle 
		@param filepath a String object containing the file path of a configuration file
		@param isFileRead a boolean variable containing whether the configuration file was already (true) or not (false)
		
		Calibrates the camera via white balancing and retrieves the work related meta data board name, component names, occurrences per component and if the component has polarity
		
		@see Native Library Documentation for further details
		@see Java_de_ur_juergenhahn_ba_Handler_NativeHandler_calibrateNative()
		
		@return an array of array of String objects containing the work related meta data board name, component names, occurrences per component and if the component has polarity
	*/
	private native static String[][] calibrateNative(long frameAddress, int ccvWidth, int ccvHeight, int srX, int srY, int srWidth, int srHeight, String filepath);
	
	/**
		\brief private native static member function
	
		@param frameAddress a long variable containing the memory address of a Mat object containing frame data
		@param componentNumber an int variable containing the number of a component to be implemented
		
		Processes the given frame in the implementation context
		
		@see Native Library Documentation for further details
		@see Java_de_ur_juergenhahn_ba_Handler_NativeHandler_processImplementationNative()
		
		@return 0 if the function was executed correctly 
	 
	*/
	private native static int processImplementationNative(long frameAddress, int componentNumber);
	
	/**
		\brief private native static member function
	
		@param frameAddress a long variable containing the memory address of a Mat object containing frame data
		@param process an int variable containing the number of the box a component is stored in
		
		Processes the given frame in the withdrawal context
		
		@see Native Library Documentation for further details
		@see Java_de_ur_juergenhahn_ba_Handler_NativeHandler_processWithdrawalNative()
		
		@return 0 if the function was executed correctly 
	 
	*/
	private native static int processWithdrawalNative(long frameAddress, int process);
		
	/**
	 	\brief public static member function
	
		@param frameAddress a long variable containing the memory address of a Mat object containing frame data
		@param ccvWidth an int variable containing the width of a custom camera view
		@param ccvHeight an int variable containing the width of a custom camera view
		@param srX an int variable containing the x coordinate of the scan rectangle
		@param srY an int variable containing the y coordinate of the scan rectangle
		@param srWidth an int variable containing the width of the scan rectangle
		@param srHeight an int variable containing the height of the scan rectangle 
		@param filepath a String object containing the file path of a configuration file
		@see calibrateNative()
		
		@return an array of arrays of String objects containing the work related meta data board name, component names, occurrences per component and if the component has polarity
	 */
	public static String[][] configurate(long frameAddress, int ccvWidth, int ccvHeight, int srX, int srY, int srWidth, int srHeight, String filepath) {
		return calibrateNative(frameAddress, ccvWidth, ccvHeight, srX, srY, srWidth, srHeight, filepath);
	}
	
	/**
		\brief public static member function
	
		@param frameAddress a long variable containing the memory address of a Mat object containing frame data
		@param componentNumber an int variable containing the number of the box a component is stored in
		
		Processes the given frame in the withdrawal context
		
		@see processWithdrawalNative()
		
		@return 0 if the function was executed correctly 
	 
	*/
	public static void processWithdrawal(long frameAddress, int componentNumber) {
		processWithdrawalNative(frameAddress, componentNumber);
	}
	
	/**
		\brief public static member function
	
		@param frameAddress a long variable containing the memory address of a Mat object containing frame data
		@param componentNumber an int variable containing the number of the box a component is stored in
		
		Processes the given frame in the implementation context
		
		@see processImplementationNative()
		
		@return 0 if the function was executed correctly 
	 
	*/
	public static void processImplementation(long frameAddress, int componentNumber) {
		processImplementationNative(frameAddress, componentNumber);
	}
	
	/**
		\brief public static member function
		
		@param frameAdress a long variable containing the memory address of a Mat object containing frame data
		@param values an array of double variables depicting the coordinates of the scan rectangle
		
		Retrieves the color channel values based on the scan rectangle measurement area and the correlated color temperature.
		
		@see measureImageValuesNative()
	
		@return an array of double variables containing the color channel values and correlated color temperature
	*/
	public static double[] measureImageValues(long frameAdress, double[] values) {
		return measureImageValuesNative(frameAdress, values);
	}
}
