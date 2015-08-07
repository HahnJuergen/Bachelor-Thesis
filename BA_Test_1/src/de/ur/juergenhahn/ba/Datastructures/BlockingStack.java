package de.ur.juergenhahn.ba.Datastructures;

import java.util.concurrent.BlockingDeque;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.TimeUnit;

import org.opencv.core.Mat;

/**
	\class BlockingStack
	
	Serves a data structure for concurrent processes
*/
public class BlockingStack  {
	private static final long TIMOUT = 150L;
	
	private BlockingDeque<Mat> stack;
	
	/**
		\brief constructor 
		
		@see stack
	*/
	public BlockingStack() {
		this.stack = new LinkedBlockingDeque<Mat>();
	}
	
	/**
	 	\brief public member function
	 	
	 	retrieves the first element of the stack if available and clears the stack
	 		 
	 	@see clear()
	 	@see stack
	 	
	 	@throws InterruptedException

		@return a Mat object containing frame data
	*/
	public Mat poll() {
		try {
			Mat mat = stack.pollFirst(TIMOUT, TimeUnit.MILLISECONDS);
			
			stack.clear();
			
			return mat;
		} catch (InterruptedException ie) {
			ie.printStackTrace();
			return null;
		}
	}
	
	/**
	 	\brief public member function
	 	
	 	gets the size of the stack
	 	
	 	@see stack
	 	
		@return an int variable containing the size of the stack
	*/
	public int size() {
		return stack.size();
	}
	
	/**
		\brief public member function
		
		@param mat a Mat object containing frame data
		
		inserts a Mat object on top of the stack
		
		@see stack
		
		@throws InterruptedException
	*/
	public void put(Mat mat) {
		try {
			stack.putFirst(mat);
		} catch (InterruptedException ie) {
			ie.printStackTrace();
		}
	}
	
	/**
		\brief public member function
		
		clears the stack
		
		@see stack
	*/
	public void clear() {
		stack.clear();
	}
}
