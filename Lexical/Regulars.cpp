#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <deque>

#include "../Helper/ErrorReport.h"
#include "../Helper/StringTokenizer.h"

#include "DefinitionsHandler.h"
#include "RegularExpTokens.h"
#include "Regulars.h"

using namespace std;


void RegularExpression::addToken(RegularExpressionToken* token){
	expressionTokens.push_back(token);
}

vector<RegularExpressionToken*> RegularExpression::getRegularExpression(){
	return expressionTokens;
}

void RegularExpression::clean(){

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
	for(const auto& singleToken : lhs.getRegularExpression()){
		addToken(singleToken);
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
	for(const auto& singleToken : lhs.getRegularExpression()){
		addToken(singleToken);
	}

	// Name
	setName(lhs.getName());

	//return the object
	return *this;
}

	/*
	  first stage
	  create expression with symbols or transitions only
	 */
bool RegularExpressionGenerator::firstPhase(){

	RegularSymbol* concSymbol = new RegularSymbol('!');
	int conc = 0;

	for(int i=2;i<lineTokens.size();i++){

		string currToken = lineTokens[i]->getData();

		switch(lineTokens[i]->getType()){

			case STRING_TOKEN_LETTERS:

				//concate symbol
				if(conc > 0){
					defintnion.addToken(concSymbol->clone());
				}
				conc = 1;
				
				//check if it's a defintnion, use it
				if(definitions.find(currToken) != definitions.end()){
					
					RegularExpression old = definitions[currToken];

					RegularSymbol* open = new RegularSymbol('(');
					RegularSymbol* close = new RegularSymbol(')');

					defintnion.addToken(open);
					for(RegularExpressionToken* tok : old.getRegularExpression()){
						defintnion.addToken(tok->clone());
					}
					defintnion.addToken(close);
				//create several transition with each char
				}else{
					for(int i=0;i<currToken.length();i++){
						//add concate
						if(i != 0){
							defintnion.addToken(concSymbol->clone());
						}
						//create the transition object and add the single char transition
						RegularTransition* singleChar = new RegularTransition();
						singleChar->addTransition(currToken[i]);
						singleChar->setTransitionName(string()+currToken[i]);
						//add to the definiton
						defintnion.addToken(singleChar);
					}
				}
			break;
			

			case STRING_TOKEN_DIGITS:
			case STRING_TOKEN_ESCAPE:

				//concate symbol
				if(conc > 0){
					defintnion.addToken(concSymbol->clone());
				}
				conc = 1;

				//lamda !!
				if(currToken == "L" && lineTokens[i]->getType() == STRING_TOKEN_ESCAPE){
					//create the transition object and add the single char transition
					RegularTransition* singleChar = new RegularTransition();
					singleChar->allowEpsilon();
					singleChar->setTransitionName("Epsilon&");
					//add to the definiton
					defintnion.addToken(singleChar);
				//multiple digits or operators like >= ==
				}else{
					for(int i=0;i<currToken.length();i++){
						//add concate
						if(i != 0){
							defintnion.addToken(concSymbol->clone());
						}
						//create the transition object and add the single char transition
						RegularTransition* singleChar = new RegularTransition();
						singleChar->setTransitionName(string()+currToken[i]);
						singleChar->addTransition(currToken[i]);
						//add to the definiton
						defintnion.addToken(singleChar);
					}
				}

			break;
			
			//this is single char, then add it
			case STRING_TOKEN_SYMBOL:

				RegularExpressionToken* singleChar;
			
				//check if it's supported symbol or not
				switch(currToken[0]){
					case '(':
					case ')':
					case '|':
					case '+':
					case '*':
					case '-':

						//starting of ( with concatenate
						if(currToken[0] == '(' && conc > 0){
							defintnion.addToken(concSymbol->clone());
							conc = 0;
						}

						//if it was | or -
						// this is binary operations so we can't put ! after them
						if(currToken[0] == '|' || currToken[0] == '-'){
							conc = 0;
						}

						singleChar = new RegularSymbol(currToken[0]);
						
						//add to the symbol
						defintnion.addToken(singleChar);

						break;

					//another symbol will be needed
					default:
						//concate symbol
						if(conc > 0){
							defintnion.addToken(concSymbol->clone());
						}
						conc = 1;

						//create the transition object and add the single char transition
						singleChar = new RegularTransition();
						((RegularTransition*)singleChar)->addTransition(currToken[0]);
						((RegularTransition*)singleChar)->setTransitionName(string()+currToken[0]);
						//add to the definiton
						defintnion.addToken(singleChar);
						break;
				}
			break;
		}
	}
	return true;
}

	/*
	 second stage
	 convert it to postfix
 	*/
bool RegularExpressionGenerator::secondPhase(){

	//stack of Regular Symbols only
	stack<RegularSymbol*> op;

	for(RegularExpressionToken* tok : defintnion.getRegularExpression()){
		
		//if it's a symbol
		if(tok->getType() == REGULAR_SYMBOL_TYPE){
			char sym = ((RegularSymbol*)tok)->getSymbol();
			switch(sym){
				
				case '+':
				case '*':
				case '|':
					//check that last one is valid
					//TODO
				case '-':
				case '!':

					if(op.empty()){
						op.push((RegularSymbol*)tok);
						break;
					}
					while(!op.empty()){

						//curr top element
						RegularSymbol* top = op.top();
						
						//if it's low then add the symbol
						if(symbolPriority(sym) > symbolPriority(top->getSymbol())){
							break;
						}

						//else the it must be + or * or -
						defintnionPostFix.addToken(top);
						op.pop();
					}
					
					//the insert it
					op.push((RegularSymbol*)tok);

				break;

				case '(':
					op.push((RegularSymbol*)tok);
					break;
				case ')':
					
					RegularSymbol* top = NULL;
					while(!op.empty()){
						
						top = op.top();
						op.pop();
						
						if(top->getSymbol() == '('){
							break;
						}

						defintnionPostFix.addToken(top);
					}

					//check correctness of parentheses
					if(top == NULL || top->getSymbol() != '('){
						error.setError("ERROR IN YOUR RULE's PARENTHESES \"()\"");
						return false;
					}

					break;
			}
		//if it's a transition
		}else if(tok->getType() == REGULAR_TRANSITION_TYPE){
			defintnionPostFix.addToken(tok);
		}

	}

	//add the rest of symbols to the postfix expression
	while(!op.empty()){
		
		RegularSymbol* top = op.top();
		op.pop();
		
		if(top->getSymbol() == '('){
			error.setError("ERROR IN YOUR RULE's PARENTHESES \"()\"");
			return false;
		}

		defintnionPostFix.addToken(top);
	}

	return true;

}
	/*
	 - third stage
	 - solve "-" 
	 - CAREFUL, In this stage some of prev objects will be deleted
	 */
bool RegularExpressionGenerator::thirdPhase(){

	deque<RegularExpressionToken*> evaluator;

	for(RegularExpressionToken* tok : defintnionPostFix.getRegularExpression()){
		if(tok->getType() == REGULAR_SYMBOL_TYPE){
			
			//check for the wanted symbol
			if(((RegularSymbol*)tok)->getSymbol() != '-'){
				evaluator.push_front(tok);
				continue;
			}
			//the stack should at least has two elements
			if(evaluator.size() < 2){
				//we could actually compansiate this error by declaring it a one letter transisiton
				RegularTransition* comp = new RegularTransition(); 
				comp->addTransition('-');
				comp->setTransitionName(string()+'-');
				evaluator.push_front(comp);
				continue;
			}

			//get the first operators from the stack top
			RegularExpressionToken* op1 = evaluator.front();
			evaluator.pop_front();
			RegularExpressionToken* op2 = evaluator.front();
			evaluator.pop_front();

			//check that both are transitions
			if(op1->getType() != REGULAR_TRANSITION_TYPE || op2->getType() != REGULAR_TRANSITION_TYPE){
				error.setError("Can't solve the \"-\" sign, Type2.");
				return false;
			}

			//cast them to transition -- pointing to the same objects in memory
			RegularTransition* T_op1 = (RegularTransition*)op1;
			RegularTransition* T_op2 = (RegularTransition*)op2;

			//check that they have chars -- not epsilon
			if(T_op1->getAvailableTransitions().size() != 1 || T_op2->getAvailableTransitions().size() != 1){
				error.setError("Can't solve the \"-\" sign, Type3.");
				return false;
			}

			//get the starting and ending point
			char C_op1 = T_op1->getAvailableTransitions()[0];
			char C_op2 = T_op2->getAvailableTransitions()[0];

			RegularTransition* T_op3 = new RegularTransition();
			T_op3->setTransitionName("["+string()+T_op1->getTransitionName()+" - "+T_op2->getTransitionName()+"]");
			//add them to the first object
			for(int i=C_op2;i<=C_op1;i++){
				T_op3->addTransition(i);
			}

			//remove the second object and add the first one to the stack again
			//delete T_op2; 			//same as delete op2;
			evaluator.push_front(T_op3);	//same as push(op1);

		}else{
			evaluator.push_front(tok);
		}
	}

	int sz = evaluator.size();
	for(int i=0;i<sz;i++){
		defintnionOptimized.addToken(evaluator.back());
		evaluator.pop_back();
	}

	return true;
}

int RegularExpressionGenerator::symbolPriority(char symbol){
	switch(symbol){
		case '+':
			return 10;
		case '*':
			return 10;
		case '-':
			return 10;
		case '!':
			return 5;
		case '|':
			return 3;
		default:
			return -1;
	}
}

RegularExpressionGenerator::RegularExpressionGenerator(const vector<RuleToken*>& lTokens, unordered_map<std::string,Regular>& defs, bool isUserDefinedRegulars) : definitions(defs){
	//set the data
	this->lineTokens = lTokens;
	this->userDefinedRegulars = defs;
	this->isUserDefinedRegulars = isUserDefinedRegulars;
}

bool RegularExpressionGenerator::generateRegularExpression(){
	
	//call the three functions sequntially
	bool tryToSolve = firstPhase() && secondPhase() && thirdPhase();

	if(!tryToSolve){
		return false;
	}

	if(isUserDefinedRegulars){
		definitions[lineTokens[0]->getData()] = defintnion;
	}

	return tryToSolve;
}

bool RegularExpressionGenerator::isError(){
	return error.isError();
}

string RegularExpressionGenerator::getError(){
	return error.getError();
}

RuleFileProductionRegularExpression RegularExpressionGenerator::getProductionRegularExpression(){
	return defintnionOptimized;
}
