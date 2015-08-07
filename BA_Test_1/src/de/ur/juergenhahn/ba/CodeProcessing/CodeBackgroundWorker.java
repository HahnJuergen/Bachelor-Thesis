package de.ur.juergenhahn.ba.CodeProcessing;

import org.opencv.core.CvException;
import org.opencv.core.Mat;

import de.ur.juergenhahn.ba.Datastructures.BlockingStack;

/**
	\class CodeBackgroundWorker
	
	Concurrently inserts and evaluates Mat objects containing frame data for QR Codes
*/
public class CodeBackgroundWorker extends Thread {	

	/**
		\brief a BlockingStack object
		
		@see de.ur.juergenhahn.ba.Datastructures.BlockingStack
	*/
    private BlockingStack frames;
    
    /**
    	\brief a CodeDector object
    	
    	@see de.ur.juergenhahn.ba.CodeProcessing.CodeDetector
    */
    private CodeDetector codeDetector;
	
    /**
    	\brief constructor
    	
    	@param codeDetector a de.ur.juergenhahn.ba.CodeProcessing.CodeDetector object
    	
    	@see codeDetector
    	@see frames
    */
	public CodeBackgroundWorker(CodeDetector codeDetector) {
		this.codeDetector = codeDetector;
		this.frames = new BlockingStack();
	}

	/**
		\brief public overriden function
		
		Concurrently evaluates a Mat object polled from the top of the blocking stack searching a QR Code.
		If a code is found the blocking stack gets cleared and the found flag is inversed
		
		@see de.ur.juergenhahn.ba.CodeProcessing.CodeDetector.processCode()
		@see de.ur.juergenhahn.ba.CodeProcessing.CodeDetector.resetCodeAvailable()
		@see de.ur.juergenhahn.ba.Datastructures.BlockingStack.clear()
		
		@throws IllegalArgumentException
		@throws CvException
	*/
	@Override
	public void run() {
		while(true) {       
			Mat frame = frames.poll();
			
			if(frame == null) {
			    continue;
			} 
			
			try {
				codeDetector.processCode(frame);
				
				if(codeDetector.hasCode()) {
					frames.clear();
					codeDetector.resetCodeAvailable();
				}
				
			} catch(IllegalArgumentException iae) {
				iae.printStackTrace();
			} catch(CvException cve) {
				cve.printStackTrace();
			}
        }
	}
	
	/**
		\brief public member function
		
		@param frame a Mat object containing frame data
		
		@see de.ur.juergenhahn.ba.Datastructures.BlockingStack.put()
	*/
	public void insertFrame(Mat frame) {
		frames.put(frame);
	}
}
