package de.ur.juergenhahn.ba.Activities;

import java.lang.reflect.Array;

import com.example.ba_test_1.R;

import de.ur.juergenhahn.ba.Handler.CameraHandler;
import de.ur.juergenhahn.ba.Views.CustomCameraView;
import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.PorterDuff.Mode;
import android.os.BatteryManager;
import android.os.Bundle;
import android.view.View;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ProgressBar;
import android.widget.TextView;

/**
	\class MainActivity
	
	An Android Activity serving as the initial entry point of the program
*/
public class MainActivity extends Activity implements OnGlobalLayoutListener {
	
	private static final int BATTERY_LEVEL_COLOR_RED = 0x77FF0000;
	private static final int BATTERY_LEVEL_COLOR_YELLOW = 0x77FFFF00;
	private static final int BATTERY_LEVEL_COLOR_GREEN = 0x7700FF00;
	private static final int BATTERY_LEVEL_BORDER_HIGH_CHARGE = 75;
	private static final int BATTERY_LEVEL_BORDER_LOW_CHARGE = 25;
	private static final int PROGESSBAR_MAXIMUM = 100;
	private static final int SIZE_ELEMENTS_ARRAY = 4;

	/**
		\brief Object of CameraHandler class
		
 		@see de.ur.juergenhahn.ba.Handler.CameraHandler
	*/
    private CameraHandler cameraHandler;
    
    /**
    	\brief Object of CustomCameraView
    	
    	@see de.ur.juergenhahn.ba.Views.CustomCameraView
    */
    private CustomCameraView mOpenCvCameraView;
    
    private TextView[] elements;  
    private ProgressBar progressBar;
    private ProgressBar progressBarSR;
    private View[] scanRectangleElements;
    private double[] scanRectangle = new double[4];
    
    /**
     	\brief private member field
    	
    	Retrieves the battery charge level via anonymous callback
    	
    	@see monitorBatteryLevel()
    */
    private BroadcastReceiver batteryInfoReciever = new BroadcastReceiver() {
		
		@Override
		public void onReceive(Context context, Intent intent) {
			monitorBatteryLevel(intent.getIntExtra(BatteryManager.EXTRA_LEVEL, 0));
		}
	};
	
	/**
 		\brief public overridden function
 		
 		References the user interface and sets the needed flags for optimal displaying and registers needed callbacks
 		
 		@see batteryInfoReciever
 		@see scanRectangleElements
 		@see mOpenCvCameraView
 		@see cameraHandler
 		@see referenceUI()
	*/
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);

    	this.registerReceiver(this.batteryInfoReciever, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
        
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.activity_main);   
        
        referenceUI();              
        
        scanRectangleElements[3].getViewTreeObserver().addOnGlobalLayoutListener(this);  
      
        mOpenCvCameraView = (CustomCameraView) findViewById(R.id.previewCamera);   
        cameraHandler = new CameraHandler(this, mOpenCvCameraView, elements, scanRectangle, scanRectangleElements, progressBar, progressBarSR);
    }    
    
    /**
    	\brief private member function
    	
    	@see elements
    	@see scanRectangleElements
    	@see progressBar
    	@see progressBarSR
    	@see PROGESSBAR_MAXIMUM
    	@see getElementsArray()
    */
	private void referenceUI() {
		 elements = getElementsArray(R.id.tvBRC, R.id.tvTRC, R.id.tvTLC, R.id.tvBLC, TextView.class);
		 scanRectangleElements = getElementsArray(R.id.indicatorTop, R.id.indicatorRight, R.id.indicatorBottom, R.id.indicatorLeft, View.class);
        
		 progressBar = (ProgressBar) findViewById(R.id.progressbar);      
		 progressBar.setProgressDrawable(getResources().getDrawable(R.drawable.custom_progressbar_horizontal));
		 progressBar.setVisibility(View.GONE);
        
		 progressBarSR = (ProgressBar) findViewById(R.id.progressbarSR);
		 progressBarSR.setVisibility(View.GONE);
		 progressBarSR.setMax(PROGESSBAR_MAXIMUM);    
	}

	/**
		\brief private templated member function
	 	@tparam T a template variable depicting the object of a class which is able to call findViewByID(int) function
	 		 	 
	 	@param idC1 an final int variable containing the id of an element
	 	@param idC2 an final int variable containing the id of an element
	 	@param idC3 an final int variable containing the id of an element
	 	@param idC4 an final int variable containing the id of an element
		@param type a final Class<T> variable containing the type of T
		
		Fills an array of T of the size 4 with the elements retrieved via the given ids based on the given type
		
		@return an array containing the 4 objects retrieved via the given ids with the type T 
		
		@see SIZE_ELEMENTS_ARRAY
	*/
	@SuppressWarnings("unchecked")
	private <T> T[] getElementsArray(final int idC1, final int idC2, final int idC3, final int idC4, Class<T> type) {
     	T[] buf = (T[]) Array.newInstance(type, SIZE_ELEMENTS_ARRAY);
     	
     	buf[0] = (T) findViewById(idC1); 
     	buf[1] = (T) findViewById(idC2); 
     	buf[2] = (T) findViewById(idC3); 
     	buf[3] = (T) findViewById(idC4);
     	
     	return (T[]) buf;
    }
	
	/**
		\brief private member function
		@param batteryLevel an int variable containing the percentage of the battery charge
	
		Shows and updates a progress bar showing the battery charge and shows the charge percentage as text.
		The color of the progress bar depends on the charge level. 
		
		@see BATTERY_LEVEL_BORDER_HIGH_CHARGE
		@see BATTERY_LEVEL_BORDER_LOW_CHARGE
		@see BATTERY_LEVEL_COLOR_GREEN
		@see BATTERY_LEVEL_COLOR_YELLOW
		@see BATTERY_LEVEL_COLOR_RED
	*/
	private void monitorBatteryLevel(int batteryLevel) {
		ProgressBar progressBarBatteryLevel = (ProgressBar) findViewById(R.id.progressbarBattery);
		
        progressBarBatteryLevel.setVisibility(View.VISIBLE);
        progressBarBatteryLevel.setMax(PROGESSBAR_MAXIMUM);
		progressBarBatteryLevel.setProgress(batteryLevel);		
		
		if(batteryLevel > BATTERY_LEVEL_BORDER_HIGH_CHARGE) {
			progressBarBatteryLevel.getProgressDrawable().setColorFilter(BATTERY_LEVEL_COLOR_GREEN, Mode.SRC_IN);
		} else if(batteryLevel > BATTERY_LEVEL_BORDER_LOW_CHARGE) {
			progressBarBatteryLevel.getProgressDrawable().setColorFilter(BATTERY_LEVEL_COLOR_YELLOW, Mode.SRC_IN);
		} else {				
			progressBarBatteryLevel.getProgressDrawable().setColorFilter(BATTERY_LEVEL_COLOR_RED, Mode.SRC_IN);
		}
		
		TextView tvBattery = (TextView) findViewById(R.id.tvBattery);
		
		tvBattery.setText(batteryLevel + getString(R.string.percent));
	}

	/**
		\brief public overridden function
		
		Called when activity is paused
		
		@see de.ur.juergenhahn.ba.Handler.CameraHandler.disable()
	*/
	@Override
    public void onPause() {
        super.onPause();
        cameraHandler.disable();
    }
    
	/**
		\brief public overridden function 
		
		Called when program is shutdown 
		
		@see de.ur.juergenhahn.ba.Handler.CameraHandler.disable()
		@see batteryInfoReciever
	*/
    @Override
    public void onDestroy() {
        super.onDestroy();
        cameraHandler.disable();
        this.unregisterReceiver(this.batteryInfoReciever);
    }

    /**
     	\brief public overridden function
     	
    	Called when activity is resumed (or started for the first time)
    	
    	@see de.ur.juergenhahn.ba.Handler.CameraHandler.initializeBackgroundTask()	

    */
	@Override
	protected void onResume() {
		super.onResume();
		cameraHandler.initializeBackgroundTask();  				
	}

	/**
		\brief public overridden function
		
		@see scanRectangleElements
		@see scanRectangle
		
		Callback called once layout creation is finished setting variables and hiding the scan rectangle views		
	*/
	@Override
	public void onGlobalLayout() {
		scanRectangleElements[3].getViewTreeObserver().removeGlobalOnLayoutListener(this);

	    int[] locations = new int[2];
	    
	    scanRectangleElements[3].getLocationOnScreen(locations);			    
	    scanRectangle[0] = locations[0];
	    scanRectangle[1] = locations[1];   
	    
	    scanRectangleElements[1].getLocationOnScreen(locations);			    
	    scanRectangle[2] = locations[0] - scanRectangle[0];
	    scanRectangle[3] = scanRectangleElements[1].getHeight();
	    
	    for(View elem : scanRectangleElements) {
	    	elem.setVisibility(View.GONE);
	    }
	}	
}