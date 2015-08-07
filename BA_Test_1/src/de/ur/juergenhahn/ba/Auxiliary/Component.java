package de.ur.juergenhahn.ba.Auxiliary;

/**
	\class Component
	
	rudimentary class to save information about components in Java
	
	@see aux::Component in C++ documentation part
*/
public class Component {
	
	private String name;
	private int occurrences;
	private boolean polarity;

	/**
		\brief constructor
		
		@param name a String object containing the name of a component
		@param occurrences an int containing the number of occurrences of the component
		@param polarity a boolean containing whether the component has polarity (true) or not (false)
	
		Basic constructor for initialization purposes
		
		@see name
		@see occurrences
		@see polarity
	*/
	public Component(String name, int occurrences, boolean polarity) {
		this.name = name;
		this.occurrences = occurrences;
		this.polarity = polarity;
	}

	/**
		\brief get the name of a component
		
		@return a String containing the name of the component
		
		@see name
	*/
	public String getName() {
		return this.name;
	}

	/**
		\brief get the number of occurrences of a component
	
		@return a int containing the number of occurrences of the component
		
		@see occurrences
	*/
	public int getOccurrences() {
		return this.occurrences;
	}

	/**
		\brief get the polarity of the component
		
		@return a boolean containing whether a component has polarity (true) or not (false)
		
		@see polarity
	*/
	public boolean hasPolarity() {
		return this.polarity;
	}
}
