#ifndef REGULAE_EXP_TOKENS_H
#define REGULAE_EXP_TOKENS_H

#include <iostream>
#include <string>
#include <vector>
#include <bitset>

#define REGULAR_TRANSITION_TYPE		0
#define REGULAR_SYMBOL_TYPE		 	1

/*
 * Class Name : RegularExpressionTokens
 *
 * This interface will be implemented by any object can exist in reqular expression.
 * Currently only a Symbol and a Transition can exist in reqular expression.
 */
class RegularExpressionTokens{

	public:
		
		// Return the Type of the Token.
		virtual int getType() = 0;

		// clone the Token by deep-copying the object.
		virtual RegularExpressionTokens* clone() = 0;
};


/*
 * Class Name : RegularTransition
 *
 * This represent a char or more in the reqular expression.
 * This called Transition as this will be a transition in the NFA.
 */
class RegularTransition : public RegularExpressionTokens{
	
	private:

		// The transition's name. used for debugging and for naming the terminal
		//nodes in the NFA & DFA. 
		std::string transitionName;

		// ASCIIset containing all the chars transitions supported by the object.
		// This used for fast testing during the insertion in O(1).
		std::bitset<128> ASCIIset;

		// All the chars supported in this transition. it's vector for fast returning.
		std::vector<char> transitions;

		// Flag to check if epsilon transition is supported or not.
		bool epsilon;

	public:

		// Constructor.
		RegularTransition();
		
		// Setter and Getter for the transition name.
		void setTransitionName(std::string name);
		std::string getTransitionName();
		
		
		// Allow or prevent the epsilon transition.
		void allowEpsilon();
		void preventEpsilon();

		// Check if the epsilon transition available or not.
		bool isEpsilonAvailable();
		
		// Add the all Transitions in the given object in this object transitions.
		void mergeTransition(RegularTransition trans);
		
		// Add the char to the supported tranisitions.
		void addTransition(char x);
		
		// Return all the available transitions in this object.
		std::vector<char> getAvailableTransitions();
		
		// Return REGULAR_TRANSITION_TYPE
		int getType() override;
		
		// Clone the current object and return new one exactly behaved like this.
		RegularTransition* clone() override;
};


/*
 * Class Name : RegularSymbols
 *
 * This represent a symbol in the reqular expression.
 * This symbol will represent an operation in the regular expression.
 */
class RegularSymbols : public RegularExpressionTokens{

	private:

		// The symbol represtening the operation in the reqular expression.
		char symbol;

	public:

		// Setter and Getter of the object.
		// If the symbol isn't recognized, getSymbol() will return 0 in ASCII.
		RegularSymbols(char sym);
		char getSymbol();

		// Return REGULAR_SYMBOL_TYPE
		std::string getType() override;

		// Clone the current object and return new one exactly behaved like this.
		RegularSymbols* clone() override;
};

#endif