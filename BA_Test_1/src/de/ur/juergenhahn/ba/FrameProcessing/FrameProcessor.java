package de.ur.juergenhahn.ba.FrameProcessing;

import org.opencv.core.Mat;

import de.ur.juergenhahn.ba.Handler.NativeHandler;

/**
	\class FrameProcessor
	
	Processes frames with work task relevant data
*/
public class FrameProcessor {		

	/**
		\brief constructor
	*/
	public FrameProcessor(){}
	
	/**
		\brief public member function
	 	
		@param m a Mat object containing frame data to be processed according to the withdrawal sub task
		@param process a int variable containing the number of the current component to be withdrawn
		
		processes the given frame for withdrawal sub task
		
		@see de.ur.juergenhahn.ba.Handler.NativeHandler.processWithdrawal()
	*/
	public void processWithdrawal(Mat m, int process) {  
		NativeHandler.processWithdrawal(m.getNativeObjAddr(), process);
	}
	
	/**
		\brief public member function
	 
		@param m a Mat object containing frame data to be processed according to the implementation sub task
		@param cn a int variable containing the number of the current component to be implemented
		
		processes the given frame for implementation sub task
		
		@see de.ur.juergenhahn.ba.Handler.NativeHandler.processImplementation()
	*/
	public void processImplementation(Mat m, int cn) {
		NativeHandler.processImplementation(m.getNativeObjAddr(), cn);
	}
}
