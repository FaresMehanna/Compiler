#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include "RegularExpTokens.h"

using namespace std;


RegularTransition::RegularTransition(){
	epsilon = false;
}

void RegularTransition::setTransitionName(string name){
	transitionName = name;
}

string RegularTransition::getTransitionName(){
	return transitionName;
}

void RegularTransition::allowEpsilon(){
	epsilon = true;
}

void RegularTransition::preventEpsilon(){
	epsilon = false;
}

bool RegularTransition::isEpsilonAvailable(){
	return epsilon;
}

void RegularTransition::mergeTransition(RegularTransition trans){
	//get list of transitions from "trans"
	vector<char> transitionsToMerge =  trans.getAvailableTransitions();

	//add them to the current object
	for(char transition : transitionsToMerge){
		this->addTransition(transition);
	}
}

void RegularTransition::addTransition(char x){
	//if char already existed, return
	if(unique.test(x)){
		return;
	}
	//else add it to the vector & bitset
	unique.set(x);
	transitions.push_back(x);
}

vector<char> RegularTransition::getAvailableTransitions(){
	return transitions;
}

string RegularTransition::getType(){
	return "Def";
}

RegularTransition* RegularTransition::clone(){
	
	RegularTransition* clone = new RegularTransition();

	if(this->isEpsilonAvailable()){
		clone->allowEpsilon();
	}
	clone->setTransitionName(this->getTransitionName());
	clone->mergeTransition(*this);

	return clone;
}



		
RegularSymbols::RegularSymbols(char sym){
	switch(sym){
		case '|':
		case '+':
		case '*':
		case '-':
		case '(':
		case ')':
		case '!':
			symbol = sym;
			break;
		default:
			cout << "ERROR ERROR ERROR in REGULAR SYMBOL" << endl;
			cout << sym << endl;
			cout << sym << endl;
			cout << sym << endl;
			break;
			//TODO
			//Error message
	}
}

RegularSymbols* RegularSymbols::clone(){
	return new RegularSymbols(this->getSymbol());
}


char RegularSymbols::getSymbol(){
	return symbol;
}

string RegularSymbols::getType(){
	return "Sym";
}
