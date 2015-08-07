package de.ur.juergenhahn.ba.Handler;

import java.util.ArrayList;
import java.util.List;
import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Mat;

import com.example.ba_test_1.R;

import android.content.Context;
import android.os.Environment;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.TextView;

import de.ur.juergenhahn.ba.Auxiliary.Component;
import de.ur.juergenhahn.ba.CodeProcessing.CodeBackgroundWorker;
import de.ur.juergenhahn.ba.CodeProcessing.CodeDetector;
import de.ur.juergenhahn.ba.FrameProcessing.ConfigurationBackgroundWorker;
import de.ur.juergenhahn.ba.FrameProcessing.FrameProcessor;
import de.ur.juergenhahn.ba.Views.CustomCameraView;

/** 
	\class CameraHandler
	
	Handles camera input and delegates each retrieved frame to further processing functions and ultimately shows each processed frame
*/
public class CameraHandler implements CameraBridgeViewBase.CvCameraViewListener2 {	
	
	private Context context;
	
	/**
		\brief a FrameProcessor object
		
		@see de.ur.juergenhahn.ba.FrameProcessing.FrameProcessor
	*/
    private FrameProcessor frameProcessor;
    
    /**
		\brief a CodeDetector object
		
		@see de.ur.juergenhahn.ba.CodeProcessing.CodeDetector
	*/
    private CodeDetector codeDetector;
    
    /**
		\brief a CodeBackgroundWorker object
		
		@see de.ur.juergenhahn.ba.CodeProcessing.CodeBackgroundWorker
	*/
    private CodeBackgroundWorker codeBackgroundWorker;    
    
    /**
    	\brief a ConfigurationBackgroundWorker object
		
		@see de.ur.juergenhahn.ba.FrameProcessing.ConfigurationBackgroundWorker
    */
    private ConfigurationBackgroundWorker configurationBackgroundWorker;
    
    /**
		\brief a CustomCameraView object
		
		@see de.ur.juergenhahn.ba.Views.CustomCameraView
	*/
    private CustomCameraView mOpenCvCameraView;

    /**
		\brief a UIUpdateHandler object
		
		@see de.ur.juergenhahn.ba.Handler.UIUpdateHandler
	*/
    private UIUpdateHandler uiUpdateHandler;
    
    private Mat currentFrame;
	private TextView[] elements;
	private double[] scanRectangleCoordinates;
	private View[] scanRectangleElements;
	private ProgressBar progressBar;
	private int process = -1;
	private boolean isWithdrawn = false;
	private ProgressBar progressBarSR;
	private String configFilePath;
	private boolean isWhiteBalanced = false;
	private String previousCode = "";
	private int maxProcessingNum;
	
	 /**
		\brief a list of Component objects
		
		@see de.ur.juergenhahn.ba.Auxiliary.Component
	*/
	private List<Component> components = new ArrayList<Component>();
	    
	/**
		\brief constructor 
	
		@param context a Context object containing the Activity context
	    @param mOpenCvCameraView a CustomCameraView object containing the camera view
	    @param elements an array of TextView objects showing data
	    @param scanRectangle an array of double variables containing the coordinates of the scan rectangle needed for calibration
	    @param scanRectangleElements an array of View objects containing the edges of the scan rectangle needed for calibration
	    @param progressBar a ProgressBar object showing the progress of the current work process
	    @param progressBarSR a ProgressBar object showing the progress of a calibration process
	    
	    sets the given variables
	    
	    @see context
	    @see mOpenCvCameraView
	    @see elements
	    @see scanRectangleCoordinates
	    @see scanRectangleElements
	    @see progressBar
	    @see progressBarSR
	*/ 
    public CameraHandler(Context context, CustomCameraView mOpenCvCameraView, TextView[] elements, double[] scanRectangle, View[] scanRectangleElements, ProgressBar progressBar, ProgressBar progressBarSR) {
        this.context = context;
        this.mOpenCvCameraView = mOpenCvCameraView;
        this.mOpenCvCameraView.setCvCameraViewListener(this);
        this.elements = elements;       
        this.scanRectangleCoordinates = scanRectangle;
        this.scanRectangleElements = scanRectangleElements;
        this.progressBar = progressBar;
        this.progressBarSR = progressBarSR;
    }

    /**
    	\private member field 
    	
    	Enables the camera view and loads the native library if successful
    	
    	@see context
    	@see mOpenCvCameraView
    */
	private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(context) {

        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS :                 	
                	System.loadLibrary(context.getString(R.string.native_library));                	
                	mOpenCvCameraView.enableView();
                break;
                
                default : super.onManagerConnected(status); break;
            }
        }
    };
    
    /**
    	\brief public member function
    	
    	Disables the camera view if it is initialized and interrupts background workers
    
    	@see mOpenCvCameraView
    	@see codeBackgroundWorker
    	@see codeBackgroundWorker
    */
    public void disable() {
        if (mOpenCvCameraView != null) { 
        	mOpenCvCameraView.disableView(); 
        }
        
        codeBackgroundWorker.interrupt();
        configurationBackgroundWorker.interrupt();
    }

    /**
     	\brief public member function
     	
    	initialize asynchronous opencv library support
    	
    	@see context
    	@see mLoaderCallback    	
    */
    public void initializeBackgroundTask() {
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_11, context, mLoaderCallback);
    }
    
    /**
    	\brief public overriden function
    	
    	@param width
    	@param height
  		
  		sets all given variables at the initial start of the camera preview and enables the background workers
  		
  		@see codeDetector
  		@see uiUpdateHandler
  		@see frameProcessor
  		@see codeBackgroundWorker
  		@see configurationBackgroundWorker
  		@see configFilePath
  		@see context
  		@see elements
  		@see scanRectangleElements
  		@see progressBar
  		@see progressBarSR
    */
    @Override
    public void onCameraViewStarted(int width, int height) {
    	codeDetector = new CodeDetector();
        uiUpdateHandler = new UIUpdateHandler(context, elements, scanRectangleElements, progressBar, progressBarSR);
        frameProcessor = new FrameProcessor();        
        codeBackgroundWorker = new CodeBackgroundWorker(codeDetector);
        configurationBackgroundWorker = new ConfigurationBackgroundWorker(mOpenCvCameraView, scanRectangleCoordinates, uiUpdateHandler);
        codeDetector.enable();
        codeBackgroundWorker.start();
    
        configFilePath = Environment.getExternalStorageDirectory().getAbsolutePath() + context.getString(R.string.configFilePath);
    }
    
    /**
    	\brief public overridden function
    	
    	shuts down the background workers once the camera preview is stopped
    	
    	@see currentFrame
    	@see codeBackgroundWorker
    	@see configurationBackgroundWorker
    */
    @Override
    public void onCameraViewStopped() {
        currentFrame.release();        
        codeBackgroundWorker.interrupt();
        configurationBackgroundWorker.interrupt();
    }
           
    /**
    	\brief public overriden function
    	
    	@param inputFrame a final CvCameraViewFrame object containing frame data
    
    	Receives each camera frame and delegates it to further processing and then displays it
    	
    	@see currentFrame
    	@see codeBackgroundWorker
    	@see de.ur.juergenhahn.ba.CodeProcessing.CodeBackgroundWorker.insertFrame()
  		@see handleConfiguration()
  		@see handleFrameProcessing()
    */
    @Override
    public Mat onCameraFrame(final CvCameraViewFrame inputFrame) { 
        currentFrame = inputFrame.rgba();   
        codeBackgroundWorker.insertFrame(currentFrame);   
        
        handleConfiguration();
        handleFrameProcessing();            
         
        return currentFrame;
    }       
    
    /**
    	\brief private member function
    	
    	calibrates the camera via white balancing after the respective QR code was found and sets the work task meta data 
    	
    	@see codeDetector
    	@see de.ur.juergenhahn.ba.CodeProcessing.CodeDetector.getCodeInterpretation()
    	@see de.ur.juergenhahn.ba.CodeProcessing.CodeDetector.enable()
    	@see configurationBackgroundWorker
    	@see applyConfiguration()
    	@see de.ur.juergenhahn.ba.FrameProcessing.ConfigurationBackgroundWorker.insertFrame()
    	@see de.ur.juergenhahn.ba.FrameProcessing.ConfigurationBackgroundWorker.size()
    	@see de.ur.juergenhahn.ba.FrameProcessing.ConfigurationBackgroundWorker.configurate()
    	@see maxProcessingNum
    	@see currentFrame
    	@see uiUpdateHandler
    	@see de.ur.juergenhahn.ba.Handler.UIUpdateHandler.updateTextViewTLC()
    	@see de.ur.juergenhahn.ba.Handler.UIUpdateHandler.hideWhiteBalancingRectangle()
    	@see de.ur.juergenhahn.ba.Handler.UIUpdateHandler.updateTextViewBRC()
    	@see components
    	@see de.ur.juergenhahn.ba.Auxiliary.Component
    	@see progressBar
    	@see isWhiteBalanced
    	@see configFilePath
    	@see context
    */
	private void handleConfiguration() {
		if(codeDetector.getCodeInterpretation().equals(context.getString(R.string.calibration)) 
			&& !configurationBackgroundWorker.isAlive()) {
			
			applyConfiguration();
		} 		
		
		if(configurationBackgroundWorker.isAlive()) {
			configurationBackgroundWorker.insertFrame(currentFrame);
		} else if(configurationBackgroundWorker.size() > 0) {	
			
			String[][] uiValues = configurationBackgroundWorker.configurate(configFilePath);			
			
			if(uiValues.length > 0) {
				maxProcessingNum = uiValues[1].length - 1;	
				String boardName = uiValues[0][0];
				uiUpdateHandler.updateTextViewTLC(boardName);		
				
				for(int i = 0; i < uiValues[1].length; i++) {
					components.add(new Component(uiValues[1][i], Integer.parseInt(uiValues[2][i]), Integer.parseInt(uiValues[3][i]) != 0));
				}
				
				progressBar.setMax(uiValues[1].length);
			} 
			
			codeDetector.enable();			
			
			isWhiteBalanced  = true;			
			uiUpdateHandler.hideWhiteBalancingRectangle();
			uiUpdateHandler.updateTextViewBRC(context.getString(R.string.calibration_success));
		} 		
	}
	
	/**
		\brief private member function
		
		handles the processing of frames based on the chosen QR Code
		
		@see whitebalanceBackgroundWorker
		@see isWhiteBalanced
		@see codeDetector
		@see de.ur.juergenhahn.ba.CodeProcessing.CodeDetector.getCodeInterpretation()
		@see previousCode
		@see handleWithdrawal()
		@see handleImplementation()
		@see context
	*/
	private void handleFrameProcessing() {
		if(!configurationBackgroundWorker.isAlive() && isWhiteBalanced) {				
			if(codeDetector.getCodeInterpretation().equals(context.getString(R.string.withdraw)) 
					|| (codeDetector.getCodeInterpretation().equals("") 
							&& previousCode.equals(context.getString(R.string.withdraw)))) {
				handleWithdrawal();
			} else if((codeDetector.getCodeInterpretation().equals(context.getString(R.string.implement)) 
					|| (codeDetector.getCodeInterpretation().equals("")
							&& previousCode.equals(context.getString(R.string.implement)))) && process > -1 ) {	
				handleImplementation();
			}
		}		
	}

	/**
		\brief private member function
		
		starts a background worker which gathers frames and notifies the user about the calibration process by updating the user interface
		
		@see codeDetector
		@see de.ur.juergenhahn.ba.CodeProcessing.CodeDetector.disable()
		@see de.ur.juergenhahn.ba.CodeProcessing.CodeDetector.reset()
		@see configurationBackgroundWorker
		@see mOpenCvCameraView
		@see scanRectangleCoordinates
		@see uiUpdateHandler
		@see de.ur.juergenhahn.ba.FrameProcessing.ConfigurationBackgroundWorker
		@see de.ur.juergenhahn.ba.Handler.UIUpdateHandler.showWhiteBalancingRectangle()
		@see de.ur.juergenhahn.ba.Handler.UIUpdateHandler.updateTextViewBRC()
		@see de.ur.juergenhahn.ba.Handler.UIUpdateHandler.displayEnteredMode()
	*/
	private void applyConfiguration() {
		codeDetector.disable();
		configurationBackgroundWorker = new ConfigurationBackgroundWorker(mOpenCvCameraView, scanRectangleCoordinates, uiUpdateHandler);
		configurationBackgroundWorker.start();      
		codeDetector.reset();		
		uiUpdateHandler.showWhiteBalancingRectangle();
		uiUpdateHandler.updateTextViewBRC(context.getString(R.string.calibrating));
		uiUpdateHandler.displayEnteredMode(context.getString(R.string.calibrate), "");
    }
	
	/**
		\brief private member function
		
		Handles the processing of frames in the context of the withdrawal task
		
		@see previousCode
		@see context
		@see isWithdrawn
		@see process
		@see maxProcessingNum
		@see uiUpdateHandler
		@see de.ur.juergenhahn.ba.Handler.UIUpdateHandler.displayEnteredMode()
		@see de.ur.juergenhahn.ba.Handler.UIUpdateHandler.updateTextViewBRC()
		@see de.ur.juergenhahn.ba.Handler.UIUpdateHandler.updateComponentInfo()
		@see de.ur.juergenhahn.ba.Handler.UIUpdateHandler.updateProcessProgess()
		@see frameProcessor
		@see de.ur.juergenhahn.ba.FrameProcessing.FrameProcessor.processWithdrawal()
	*/
	private void handleWithdrawal() {
		previousCode = context.getString(R.string.withdraw);
		
		if(!isWithdrawn) {
			
			if(process == maxProcessingNum) {
				process = -1;
				uiUpdateHandler.displayEnteredMode(context.getString(R.string.completion), "");
			}
					
			process = (process == maxProcessingNum) ? -1 : process;
			process++;		
			isWithdrawn = true;
			uiUpdateHandler.displayEnteredMode(context.getString(R.string.withdraw), components.get(process).getName());
		}
		
		frameProcessor.processWithdrawal(currentFrame, process);
		uiUpdateHandler.updateTextViewBRC(context.getString(R.string.withdrawing));
		uiUpdateHandler.updateComponentInfo(components.get(process));
		uiUpdateHandler.updateProcessProgess(process + 1, maxProcessingNum + 1);
	}
	
	/**
		\brief private member function
		
		Handles the processing of frames in the context of the implementation task
		
		@see isWithdrawn
		@see uiUpdateHandler
		@see de.ur.juergenhahn.ba.Handler.UIUpdateHandler.displayEnteredMode()
		@see de.ur.juergenhahn.ba.Handler.UIUpdateHandler.updateTextViewBRC()
		@see context
		@see components
		@see previousCode
		@see frameProcessor
		@see de.ur.juergenhahn.ba.FrameProcessing.FrameProcessor.processImplementation()
		@see currentFrame
		@see process
	*/
	private void handleImplementation() {
		if(isWithdrawn) {
			uiUpdateHandler.displayEnteredMode(context.getString(R.string.implement), components.get(process).getName());			
		}
		
		previousCode = context.getString(R.string.implement);
		frameProcessor.processImplementation(currentFrame, process);  
		isWithdrawn = false;
		uiUpdateHandler.updateTextViewBRC(context.getString(R.string.implementing));
	}
}
