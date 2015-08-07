package de.ur.juergenhahn.ba.Views;

import android.content.Context;
import android.hardware.Camera;
import android.util.AttributeSet;
import android.view.SurfaceHolder;

/**
	\class CustomCameraView
	
	Serves as the view needed for displaying the camera preview
*/
public class CustomCameraView extends org.opencv.android.JavaCameraView implements SurfaceHolder.Callback {

	/**
	 	\brief constructor
	 	
		@param context a Context object containing the activity context
		@param attrs a AttributeSet object containing camera attributes
		
		Calls the super constructor of JavaCameraView
	*/
	public CustomCameraView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }
    
	/**
	 	\brief public member function
		
		@param isLocked a boolean variable which sets a lock (true) or lifts it (false)
		
		Sets a lock (true) to the camera's auto white balancing feature or lifts it (false)
	*/
    public void setAutoWhiteBalanceLock(boolean isLocked) {
    	 Camera.Parameters params = mCamera.getParameters();
         params.setAutoWhiteBalanceLock(isLocked);
         mCamera.setParameters(params);
    }     
    
    /**
     	\brief public member function
		
		@param isLocked a boolean variable which sets a lock (true) or lifts it (false)
		
		Sets a lock (true) to the camera's auto exposure feature or lifts it (false)
    */
    public void setAutoExposureLock(boolean isLocked) {
    	Camera.Parameters params = mCamera.getParameters();
        params.setAutoExposureLock(isLocked);
        mCamera.setParameters(params);
    }
}
