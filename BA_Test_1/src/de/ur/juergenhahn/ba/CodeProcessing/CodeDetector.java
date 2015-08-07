package de.ur.juergenhahn.ba.CodeProcessing;

import org.opencv.android.Utils;
import org.opencv.core.Mat;

import android.graphics.Bitmap;
import com.google.zxing.BinaryBitmap;
import com.google.zxing.ChecksumException;
import com.google.zxing.FormatException;
import com.google.zxing.LuminanceSource;
import com.google.zxing.NotFoundException;
import com.google.zxing.RGBLuminanceSource;
import com.google.zxing.Reader;
import com.google.zxing.Result;
import com.google.zxing.common.HybridBinarizer;
import com.google.zxing.qrcode.QRCodeReader;

/**
	\class CodeDetector
	
	evaluates frames for QR Codes and retrieves their data
*/
public class CodeDetector {
	
	private boolean hasCode = false;
	private String code = "";
	private boolean isEnabled;
	
	/**
		\brief constructor
	*/
	public CodeDetector() {}

	/**
		\brief public member function
		
		@return a boolean which contains whether a QR code has been found (true) or not (false)
		
		@see hasCode
	*/
	public boolean hasCode() {		
		return hasCode;
	}

	/**
	 	\brief public member function
	 	
		@param mRgba a Mat object containing frame data

		evaluates the received Mat object for QR Codes
		
		@see isEnabled
		@see detectCode()
		@see getDecodableBinaryBitmap()
	*/
	public void processCode(Mat mRgba) {
		if(isEnabled) {
			if(detectCode(getDecodableBinaryBitmap(mRgba))) return;       
		}
	}	
	
	/**	
		\brief private member function
		
		@param bitmap a BinaryBitmap object containing frame data
		
		Searches a QR Code in the given bitmap
		
		@see code
		@see hasCode
		
		@throws ChecksumException
		@throws FormatException
		@throws NotFoundException
		
		@return true if a QR Code was successfully found and decoded, false else
	*/
	private boolean detectCode(BinaryBitmap bitmap) {
		Reader reader = new QRCodeReader();
	
        Result result = null;
        
		try {		
			result = reader.decode(bitmap);						
            code = result.getText();	
                       
            reader.reset();    
                       
            hasCode = true;    
            
            return true;
		} catch (ChecksumException ce) {
			hasCode = false;
		} catch (FormatException fe) {
			hasCode = false;
		} catch (NotFoundException nfe) {
			hasCode = false;
		}
        
        reader.reset();
        
        return false;
	}
	
	/**
		\brief private member function
		
		@param mRgba a Mat object containing frame data
		
		Converts a Mat object to a BinaryBitmap object
		
		@return a BinaryBitmap containing the data of the given Mat object which was converted
	*/
	private BinaryBitmap getDecodableBinaryBitmap(Mat mRgba) {				
		Bitmap bMap = Bitmap.createBitmap(mRgba.width(), mRgba.height(), Bitmap.Config.ARGB_8888);
		Utils.matToBitmap(mRgba, bMap);		
		
        int[] intArray = new int[bMap.getWidth() * bMap.getHeight()];

        bMap.getPixels(intArray, 0, bMap.getWidth(), 0, 0, bMap.getWidth(), bMap.getHeight());

        LuminanceSource source = new RGBLuminanceSource(bMap.getWidth(), bMap.getHeight(), intArray);

        return new BinaryBitmap(new HybridBinarizer(source));		
	}
	
	/**
		\brief resets the code variable to an empty String
		
		@see code
	*/
	public void reset() {
		code = "";
	}

	/**
	 	\brief get the data of the QR Code
	 	
		@return a String containing the data of the QR Code
	*/
	public String getCodeInterpretation() {
		return code;
	}
	
	/**
		\brief resets the hasCode to false
		
		@see hasCode
	*/
	public void resetCodeAvailable() {
		hasCode = false;
	}

	/**
		\brief enables QR Code detection by setting isEnabled to true
		
		@see isEnabled
	*/
	public void enable() {
		isEnabled = true;
	}

	/**
		\brief disables QR Code detection by setting isEnabled to false
		
		@see isEnabled
	*/
	public void disable() {
		isEnabled = false;
	}
}
