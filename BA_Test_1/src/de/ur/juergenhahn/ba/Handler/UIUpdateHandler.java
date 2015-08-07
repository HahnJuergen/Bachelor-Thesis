package de.ur.juergenhahn.ba.Handler;

import de.ur.juergenhahn.ba.Auxiliary.Component;

import com.example.ba_test_1.R;
import android.app.Activity;
import android.content.Context;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

/**
	\class UIUpdateHandler
	
	 Serves as a function wrapper to display retrieved information in the UI-Thread if the data was retrieved in another thread
*/
public class UIUpdateHandler {
	private Context context;
	private TextView[] elements;
	private ProgressBar progressBar;
	private ProgressBar progressBarSR;
	private View[] scanRectangleElements;
	
	/**
	 	\brief constructor
	 
		@param context a Context object containing the activity context
		@param elements	an array of TextView objects used to write show text in the UI
		@param scanRectangleElements an array of View objects used to visualize the scan rectangle
		@param progressBar a ProgressBar object showing the process of the work task
		@param progressBarSR a ProgressBar object showing the process of the camera calibration 
	
		@see context
		@see elements
		@see scanRectangleElements
		@see progressBar
		@see progressBarSR
	*/
	public UIUpdateHandler(Context context, TextView[] elements, View[] scanRectangleElements, ProgressBar progressBar, ProgressBar progressBarSR) {
		this.context = context;
		this.elements = elements;
		this.scanRectangleElements = scanRectangleElements;
		this.progressBar = progressBar;
		this.progressBarSR = progressBarSR;
	}
	
	/**
		\brief public member function
		
		Shows the scan rectangle in the user interface
		
		@see context
		@see scanDisplayalRunnable()
	*/
	public void showWhiteBalancingRectangle() {
		((Activity) context).runOnUiThread(scanDisplayalRunnable(true));		
	}

	/**
		\brief public member function
		
		Hides the scan rectangle in the user interface
		
		@see context
		@see scanDisplayalRunnable()
	*/
	public void hideWhiteBalancingRectangle() {
		((Activity) context).runOnUiThread(scanDisplayalRunnable(false));
	}

	/**
		\brief public member function
		
		@param text a final String object used for updating the bottom right corner TextView object
		
		Updates the TextView object at the bottom right corner of the user interface
		
		@see context
		@see elements
		@see textViewRunnable()
	*/
	public void updateTextViewBRC(final String text) {
		((Activity) context).runOnUiThread(textViewRunnable(elements[0], text));
	}

	/**
		\brief public member function
		
		@param text a final String object used for updating the top left corner TextView object
		
		Updates the TextView object at the top left corner of the user interface
		
		@see context
		@see elements
		@see textViewRunnable()
	*/
	public void updateTextViewTLC(final String text) {
		((Activity) context).runOnUiThread(textViewRunnable(elements[2], context.getString(R.string.board) + text));	
	}

	/**
		\brief public member function
		
		@param component a de.ur.juergenhahn.ba.Auxiliary.Component object holding rudimentary meta data about a component
		
		Updates the TextView object responsible for showing component information with the component information
		
		@see context
		@see elements
		@see textViewRunnable()
	*/
	public void updateComponentInfo(Component component) {
		final String name = component.getName();
		final int occurrences = component.getOccurrences();		
		final boolean hasPolarity = component.hasPolarity();
		
		String yes = context.getString(R.string.yes);
		String no = context.getString(R.string.no);
		String pol = hasPolarity ? yes : no;
		String comp = context.getString(R.string.component);
		String occurrence = context.getString(R.string.occurrences);
		String polarity = context.getString(R.string.polarity);
		String newLine = context.getString(R.string.new_line);

		final String text = comp + name + newLine +
			occurrence + occurrences + newLine +
			polarity + pol;
		
		((Activity) context).runOnUiThread(textViewRunnable(elements[3], text));
	}

	/**
	 	\brief public member function
	 
		@param currentProgress a final int object containing the current number of a processed component
		@param maximalProgress a final int object containing the number of all different kinds of components
		
		Updates the TextView object responsible for showing the number of the component and updates the ProgressBar object to show correct progress
		
		@see context
		@see elements
		@see textViewRunnable()
		@see processProgressionRunnable()
	*/
	public void updateProcessProgess(final int currentProgress, final int maximalProgress) {
		final String text = context.getString(R.string.progress) + currentProgress + context.getString(R.string.slash)+ maximalProgress;
		
		((Activity) context).runOnUiThread(textViewRunnable(elements[1], text));		
		((Activity) context).runOnUiThread(processProgressionRunnable(currentProgress));
	}

	/**
		\brief public member function
		
		@param mode a final String object containing the name of the entered program mode (calibration, withdrawal or implementation)
		@param subtext a final String object containing the name of the component to be worked with
		
		Shows an Android Toast showing the program mode and active component
		
		@see context
		@see toastRunnable()
	*/
	public void displayEnteredMode(final String mode, final String subtext) {
		((Activity) context).runOnUiThread(toastRunnable(mode, subtext));
	}

	/**
	 	\brief public member function
	 	
		@param progress a final int containing the progress of the calibration process in percent
		
		Updates the calibration ProgressBar object with the given data
		 
		@see context
		@see calibrationProgressRunnable()
	*/
	public void updateCalibrationProgress(final int progress) {
		((Activity) context).runOnUiThread(calibrationProgressRunnable(progress));
	}
	
	/**
		\brief private member function
		
		@param progress a final int containing the new value of the ProgressBar object in percent
		
		Updates the ProgressBar object responsible for showing the work process progress in the UI-Thread
		
		@see progressBar
		
		@return a Runnable object to be executed on the user interface thread which updates the ProgressBar object responsible for showing the work process progress
	*/
	private Runnable processProgressionRunnable(final int progress) {
		return new Runnable() {
			
			@Override
			public void run() {
				progressBar.setVisibility(View.VISIBLE);
				progressBar.setProgress(progress);	
			}
		};
	}
	
	/**
		\brief private member function
	 
		@param progress a final int containing the number of the component
		
		Updates the ProgressBar object responsible for showing the calibration process progress in the UI-Thread
		
		@see progressBarSR
		
		@return a Runnable object to be executed on the user interface thread which updates the ProgressBar object responsible for showing the calibration process progress
	*/
	private Runnable calibrationProgressRunnable(final int progress) {
		return new Runnable() {
			
			@Override
			public void run() {
				if(progress == 1) progressBarSR.setVisibility(View.VISIBLE);
				
				progressBarSR.setProgress(progress);
				
				if(progress == 100) progressBarSR.setVisibility(View.GONE);	
			}
		};
	}
	
	/**
		\brief private member function
		 
		@param tv a final TextView object which text is to be updated
		@param text a final String object containing the new information for the TextView object
		
		Updates the text of a given TextView with the given String object
			
		@return a Runnable object to be executed on the user interface thread which updates the text of a given TextView object with the given String object
	*/
	private Runnable textViewRunnable(final TextView tv, final String text) {
		return new Runnable() {
			
			@Override
			public void run() {
				tv.setText(text);
			}
		};
	}
	
	/**
		\brief private member function
		
		@param mode a final String containing the mode of the program (calibration, withdrawal or implementation)
		@param subtext a final String containing the name of the active component
		
		Shows a toast which contains the given data
		
		@see context
		
		@return a Runnable object to be executed on the user interface thread which shows a toast containing the given data
	*/
	private Runnable toastRunnable(final String mode, final String subtext) {
		return new Runnable() {
			
			@Override
			public void run() {
				LayoutInflater inflater =  (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
				View layout = inflater.inflate(R.layout.toast_message, (ViewGroup) ((Activity) context).findViewById(R.id.toast_layout_root));
				TextView text1 = (TextView) layout.findViewById(R.id.text1);
				text1.setText(mode);
				
				TextView text2 = (TextView) layout.findViewById(R.id.text2);
				text2.setText(subtext);
				
				Toast t = new Toast(context);
				t.setGravity(Gravity.CENTER_VERTICAL, 0, 0);
				t.setDuration(Toast.LENGTH_SHORT);
				t.setView(layout);
				t.show();
			}
		};
	}
	
	/**
		\brief private member function
		
		@param show a final boolean which shows (true) or hides (false) the scan rectangle
		
		Shows or hides the scan rectangle based on the given data
		
		@return a Runnable object to be executed on the user interface thread which shows or hides the scan rectangle
	*/
	private Runnable scanDisplayalRunnable(final boolean show) {
		return new Runnable() {
			
			@Override
			public void run() {
				if(show) {					
					for(View v : scanRectangleElements)
						v.setVisibility(View.VISIBLE);
				} else {
					for(View v : scanRectangleElements)
						v.setVisibility(View.GONE);
				}
			}
		};
	}
}
