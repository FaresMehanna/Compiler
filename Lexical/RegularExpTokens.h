#ifndef REGULAE_EXP_TOKENS_H
#define REGULAE_EXP_TOKENS_H

#include <iostream>
#include <string>
#include <vector>
#include <bitset>

class RegularExpressionTokens{
	public:
		virtual std::string getType() = 0;
		virtual RegularExpressionTokens* clone() = 0;
};

class RegularTransition : public RegularExpressionTokens{
	
	private:
		std::string transitionName;
		std::bitset<128> unique;
		std::vector<char> transitions;
		bool epsilon;
		bool star;
		bool plus;

	public:
		RegularTransition();
		void setTransitionName(std::string name);
		std::string getTransitionName();
		void allowEpsilon();
		void preventEpsilon();
		bool isEpsilonAvailable();
		void mergeTransition(RegularTransition trans);
		void addTransition(char x);
		std::vector<char> getAvailableTransitions();
		std::string getType();
		RegularTransition* clone();
};

class RegularSymbols : public RegularExpressionTokens{
	private:
		char symbol;

	public:
		RegularSymbols() = delete;
		RegularSymbols(char sym);
		char getSymbol();
		std::string getType();
		RegularSymbols* clone();
};

#endif