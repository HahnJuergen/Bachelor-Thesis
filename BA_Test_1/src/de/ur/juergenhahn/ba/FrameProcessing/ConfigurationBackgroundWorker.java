package de.ur.juergenhahn.ba.FrameProcessing;

import org.opencv.core.Mat;

import de.ur.juergenhahn.ba.Datastructures.BlockingStack;
import de.ur.juergenhahn.ba.Handler.NativeHandler;
import de.ur.juergenhahn.ba.Handler.UIUpdateHandler;
import de.ur.juergenhahn.ba.Views.CustomCameraView;

/**
	\class ConfigurationBackgroundWorker
	
	Handles the calibration of the camera via white balancing and configuration of the program via a configuration file
*/
public class ConfigurationBackgroundWorker extends Thread {			
	private static final int WAIT_TIME = 20;
	private static final int CALIBRATION_PROGRESS_ITERATIONS = 100;
	
	/**
		\brief a BlockingStack object
		
		@see de.ur.juergenhahn.ba.Datastructures.BlockingStack
	*/
	private BlockingStack stack = new BlockingStack();
	
	/**
		\brief a CustomCameraView object
		
		@see de.ur.juergenhahn.ba.Views.CustomCameraView
	 */
	private CustomCameraView ccv;
	private double[] scanRectangleCoordinates;
	
	/**
		\brief a UIUpdateHandler object
		
		@see de.ur.juergenhahn.ba.Handler.UIUpdateHandler		
	*/
	private UIUpdateHandler uiUpdateHandler;
	
	/**
	 	\brief constructor
	 	
		@param ccv a CustomCameraView object containing the camera view
	    @param scanRectangleCoordinates a double array containing the coordinates of the scan rectangle used to measure color
	    @param uiUpdateHandler UIUpdateHandler object used to update the user interface
	    
	    @see ccv
	    @see scanRectangleCoordinates
	    @see uiUpdateHandler
	 */
	public ConfigurationBackgroundWorker(CustomCameraView ccv, double[] scanRectangleCoordinates, UIUpdateHandler uiUpdateHandler) {
		this.ccv = ccv;
		this.scanRectangleCoordinates = scanRectangleCoordinates;
		
		this.uiUpdateHandler = uiUpdateHandler;
	}

	/**
		\brief public member function
		
		updates the calibration progress bar
		
		@see ccv
		@see de.ur.juergenhahn.ba.Handler.UIUpdateHandler.updateCalibrationProgress()
		@see de.ur.juergenhahn.ba.Views.CustomCameraView.setAutoExposureLock()
		@see de.ur.juergenhahn.ba.Views.CustomCameraView.setAutoWhiteBalanceLock()
		@see WAIT_TIME
		@see CALIBRATION_PROGRESS_ITERATIONS	
		
		@throws InterruptedException
	*/
	@Override
	public void run() {			
		ccv.setAutoExposureLock(false);
		ccv.setAutoWhiteBalanceLock(false);		
		
		for(int i = 1; i <= CALIBRATION_PROGRESS_ITERATIONS; i++) {
			try {
				uiUpdateHandler.updateCalibrationProgress(i);
				ConfigurationBackgroundWorker.sleep(WAIT_TIME);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}						
		}
		
		ccv.setAutoExposureLock(true);
		ccv.setAutoWhiteBalanceLock(true);	
	}
	
	/**
	 	\brief public member function 
	 	
		@param filepath a String object containing the path to a configuration file
	 	@see de.ur.juergenhahn.ba.Handler.NativeHandler.configurate()
		@see stack
		@see de.ur.juergenhahn.ba.Datastructures.BlockingStack.poll()

	 	@return an array of arrays of string object containing the work task related meta data
	 */
	public String[][] configurate(String filepath) {
		return NativeHandler.configurate(stack.poll().getNativeObjAddr(), ccv.getWidth(), ccv.getHeight(), (int) scanRectangleCoordinates[0], (int) scanRectangleCoordinates[1], (int) scanRectangleCoordinates[2], (int) scanRectangleCoordinates[3], filepath);
	}
	
	/**
	 	\brief public member function
	 	
		@param mat a Mat object containing the frame data
		
		Inserts a Mat object to the blocking stack
		
		@see stack
	*/
	public void insertFrame(Mat mat) {
		stack.put(mat);
	}
	
	/**
	 	\brief public member function
	 
	 	@see stack
	 	@see de.ur.juergenhahn.ba.Datastructures.BlockingStack.size()
	 
	 	gets the size of the blocking size
	 
		@return the size of the blocking stack
	*/
	public int size() {
		return stack.size();
	}
}
