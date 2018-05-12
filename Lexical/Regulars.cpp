#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <deque>

#include "../Helper/ErrorReport.h"
#include "../Helper/StringTokenizer.h"

#include "RegularExpTokens.h"
#include "Regulars.h"

using namespace std;


/*
 * The implementation of "RegularExpression".
 */

void RegularExpression::addToken(RegularExpressionToken* token){
	expressionTokens.push_back(token);
}

vector<RegularExpressionToken*> RegularExpression::getRegularExpression(){
	return expressionTokens;
}

void RegularExpression::clean(){

	//free all the elemnts
	for(RegularExpressionToken* singleToken : expressionTokens){
		delete(singleToken);
	}

	//clean the vector and the name
	expressionTokens.clean();
	name = "";

}

void RegularExpression::setName(string name){
	this->name = name;
}

string RegularExpression::getName(){
	return name;
}

RegularExpression::RegularExpression(const RegularExpression& lhs){

	// Deep copy all the elements
	for(RegularExpressionToken* singleToken : expressionTokens){
		addToken(singleToken->clone());
	}

	// Copy the name
	setName(lhs.getName());
}

RegularExpression& operator= (const RegularExpression& lhs){

	// Clean the prev data
	clean();

	// Deep copy all the elements
	for(RegularExpressionToken* singleToken : lhs.getRegularExpression()){
		addToken(singleToken->clone());
	}

	// Copy the name
	setName(lhs.getName());

	//return the same object
	return *this;
}

~RegularExpression(){
	clean();
}

RegularExpression(const RuleFileProductionRegularExpression& lhs){

	// Deep copy all the elements
	for(RegularExpressionToken* singleToken : lhs.getRegularExpression()){
		addToken(singleToken->clone());
	}

	// Copy the name
	setName(lhs.getName());
	
}

RegularExpression& operator= (const RuleFileProductionRegularExpression& lhs){
	// Clean the prev data
	clean();

	// Deep copy all the elements
	for(RegularExpressionToken* singleToken : lhs.getRegularExpression()){
		addToken(singleToken->clone());
	}

	// Copy the name
	setName(lhs.getName());

	//return the same object
	return *this;
}

RegularExpression(const RuleFileUserDefinedRegularExpression& lhs){

	// Deep copy all the elements
	for(RegularExpressionToken* singleToken : lhs.getRegularExpression()){
		addToken(singleToken->clone());
	}

	// Copy the name
	setName(lhs.getName());

}

RegularExpression& operator= (const RuleFileUserDefinedRegularExpression& lhs){
	// Clean the prev data
	clean();

	// Deep copy all the elements
	for(RegularExpressionToken* singleToken : lhs.getRegularExpression()){
		addToken(singleToken->clone());
	}

	// Copy the name
	setName(lhs.getName());

	//return the same object
	return *this;
}


/*
 * The implementation of "RuleFileProductionRegularExpression".
 */

RuleFileProductionRegularExpression::RuleFileProductionRegularExpression(){
	priority = 0;
}


void RuleFileProductionRegularExpression::setPriority(unsigned int pr){
	priority = pr;
}

unsigned int RuleFileProductionRegularExpression::getPriority(){
	return priority;
}

unsigned int RuleFileProductionRegularExpression::getPriority(){
	RegularExpression::clean();
	priority = 0;
}

RuleFileProductionRegularExpression::RuleFileProductionRegularExpression(const RegularExpression& lhs){
	
	// Priority
	priority = 0;

	// Tokens Pointers
	for(RegularExpressionToken* singleToken : lhs.getRegularExpression()){
		addToken(singleToken->clone());
	}

	// Name
	setName(lhs.getName());
}

RuleFileProductionRegularExpression::RuleFileProductionRegularExpression& operator= (const RegularExpression& lhs){
	
	// Clean all the data
	clean();

	// Priority
	priority = 0;

	// Tokens Pointers
	for(RegularExpressionToken* singleToken : lhs.getRegularExpression()){
		addToken(singleToken->clone());
	}

	// Name
	setName(lhs.getName());

	//return the object
	return *this;
}


/*
 * The implementation of "RuleFileUserDefinedRegularExpression".
 */

RuleFileUserDefinedRegularExpression::RuleFileUserDefinedRegularExpression(const RegularExpression& lhs){
	
	// Tokens Pointers
	for(RegularExpressionToken* singleToken : lhs.getRegularExpression()){
		addToken(singleToken->clone());
	}

	// Name
	setName(lhs.getName());
}

RuleFileUserDefinedRegularExpression::RuleFileUserDefinedRegularExpression& operator= (const RegularExpression& lhs){
	
	// Tokens Pointers
	for(RegularExpressionToken* singleToken : lhs.getRegularExpression()){
		addToken(singleToken->clone());
	}

	// Name
	setName(lhs.getName());

	//return the object
	return *this;
}