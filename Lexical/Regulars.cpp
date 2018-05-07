#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <deque>
#include "../Helper/ErrorReport.h"
#include "RuleTokenize.h"
#include "DefinitionsHandler.h"
#include "RegularExpTokens.h"
#include "Regulars.h"

using namespace std;

void Regular::addToken(RegularExpressionTokens* token){
	this->exp.push_back(token);
}

vector<RegularExpressionTokens*> Regular::getDefinition(){
	return this->exp;
}



void Regular::setName(string name){
	this->name = name;
}
string Regular::getName(){
	return name;
}


void Regular::setPriority(unsigned int pr){
	priority = pr;
}

unsigned int Regular::getPriority(){
	return priority;
}



	/*
	  first stage
	  create expression with symbols or transitions only
	 */
bool RegularGenerator::firstPhase(){

	RegularSymbols* concSymbol = new RegularSymbols('!');
	int conc = 0;

	for(int i=2;i<lineTokens.size();i++){

		string currToken = lineTokens[i]->getData();

		switch(lineTokens[i]->getType()){

			case RULE_TOKEN_LETTERS:
				//concate symbol
				if(conc > 0){
					defintnion.addToken(concSymbol->clone());
				}
				conc++;
				//check if it's a defintnion, use it
				if(definitions.find(currToken) != definitions.end()){
					
					Regular old = definitions[currToken];

					RegularSymbols* open = new RegularSymbols('(');
					RegularSymbols* close = new RegularSymbols(')');

					defintnion.addToken(open);
					for(RegularExpressionTokens* tok : old.getDefinition()){
						defintnion.addToken(tok->clone());
					}
					defintnion.addToken(close);
				//create several transition with each char
				}else{
					for(int i=0;i<currToken.length();i++){
						conc = 1;
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
			

			case RULE_TOKEN_DIGITS:
			case RULE_TOKEN_SYMBOLS:
				//concate symbol
				if(conc > 0){
					defintnion.addToken(concSymbol->clone());
				}
				conc++;
				//if single char, then create transition with single char
				if(currToken.length() == 1){
					//create the transition object and add the single char transition
					RegularTransition* singleChar = new RegularTransition();
					singleChar->addTransition(currToken[0]);
					singleChar->setTransitionName(string()+currToken[0]);
					//add to the definiton
					defintnion.addToken(singleChar);
				//if multiple chars, then check if it's a definition or a concated string
				}else{
					//lamda !!
					if(currToken == "L="){
						//create the transition object and add the single char transition
						RegularTransition* singleChar = new RegularTransition();
						singleChar->allowEpsilon();
						singleChar->setTransitionName("Epsilon&");
						//add to the definiton
						defintnion.addToken(singleChar);
					//multiple digits or operators like >= ==
					}else{
						for(int i=0;i<currToken.length();i++){
							conc = 1;
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
				}
			break;
			
			//this is single char, then add it
			default:
				RegularExpressionTokens* singleChar;
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
						//if it was | 
						if(currToken[0] == '|' || currToken[0] == '-'){
							conc = 0;
						}

						singleChar = new RegularSymbols(currToken[0]);
						//add to the symbol
						defintnion.addToken(singleChar);
						break;
					//another symbol will be needed
					default:
						//concate symbol
						if(conc > 0){
							defintnion.addToken(concSymbol->clone());
						}
						conc++;
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
bool RegularGenerator::secondPhase(){

	//stack of Regular Symbols only
	stack<RegularSymbols*> op;

	for(RegularExpressionTokens* tok : defintnion.getDefinition()){
		
		//if it's a symbol
		if(tok->getType() == "Sym"){
			char sym = ((RegularSymbols*)tok)->getSymbol();
			switch(sym){
				
				case '+':
				case '*':
				case '|':
					//check that last one is valid
					//TODO
				case '-':
				case '!':

					if(op.empty()){
						op.push((RegularSymbols*)tok);
						break;
					}
					while(!op.empty()){

						//curr top element
						RegularSymbols* top = op.top();
						
						//if it's low then add the symbol
						if(symbolPriority(sym) > symbolPriority(top->getSymbol())){
							break;
						}

						//else the it must be + or * or -
						defintnionPostFix.addToken(top);
						op.pop();
					}
					
					//the insert it
					op.push((RegularSymbols*)tok);

				break;

				case '(':
					op.push((RegularSymbols*)tok);
					break;
				case ')':
					
					RegularSymbols* top = NULL;
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
		}else if(tok->getType() == "Def"){
			defintnionPostFix.addToken(tok);
		}

	}

	//add the rest of symbols to the postfix expression
	while(!op.empty()){
		
		RegularSymbols* top = op.top();
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
bool RegularGenerator::thirdPhase(){

	deque<RegularExpressionTokens*> evaluator;

	for(RegularExpressionTokens* tok : defintnionPostFix.getDefinition()){
		if(tok->getType() == "Sym"){
			
			//check for the wanted symbol
			if(((RegularSymbols*)tok)->getSymbol() != '-'){
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
			RegularExpressionTokens* op1 = evaluator.front();
			evaluator.pop_front();
			RegularExpressionTokens* op2 = evaluator.front();
			evaluator.pop_front();

			//check that both are transitions
			if(op1->getType() != "Def" || op2->getType() != "Def"){
				cout << "ERR2" << endl; 
				error.setError("Can't solve the \"-\" sign, Type2.");
				return false;
			}

			//cast them to transition -- pointing to the same objects in memory
			RegularTransition* T_op1 = (RegularTransition*)op1;
			RegularTransition* T_op2 = (RegularTransition*)op2;

			//check that they have chars -- not epsilon
			if(T_op1->getAvailableTransitions().size() != 1 || T_op2->getAvailableTransitions().size() != 1){
				cout << "ERR3" << endl; 
				cout << T_op1->getAvailableTransitions().size() << endl; 
				cout << T_op2->getAvailableTransitions().size() << endl; 
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

int RegularGenerator::symbolPriority(char symbol){
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

RegularGenerator::RegularGenerator(vector<RuleToken*> lTokens, unordered_map<std::string,Regular>& defs, bool addToDefinitions) : definitions(defs){
	//set the data
	this->lineTokens = lTokens;
	this->definitions = defs;
	this->addToDefinitions = addToDefinitions;
}

bool RegularGenerator::generateRegular(){
	
	//call the three functions sequntially
	bool tryToSolve = firstPhase() && secondPhase() && thirdPhase();

	if(!tryToSolve){
		cout << getError() << endl;
		cout << getError() << endl;
		cout << getError() << endl;
		cout << getError() << endl;
	}

	if(tryToSolve && addToDefinitions){
		definitions[lineTokens[0]->getData()] = defintnion;
	}

	if(tryToSolve){}
		//debugInfo();

	return tryToSolve;
}

bool RegularGenerator::isError(){
	return error.isError();
}

string RegularGenerator::getError(){
	return error.getError();
}

Regular RegularGenerator::getRegular(){
	return defintnionOptimized;
}




void RegularGenerator::debugInfo(){

	for(RegularExpressionTokens* tok : defintnion.getDefinition()){
		if(tok->getType() == "Sym"){
			cout << ((RegularSymbols*)tok)->getSymbol() << " ";
		}else{
			if(((RegularTransition*)tok)->getAvailableTransitions().size() > 0){
				cout << (((RegularTransition*)tok)->getAvailableTransitions())[0] << " ";
			}else{
				if(((RegularTransition*)tok)->isEpsilonAvailable()){
					cout << "&epsilon&" << " ";
				}
			}
		}
	}
	cout << endl << endl;
	
	for(RegularExpressionTokens* tok : defintnionPostFix.getDefinition()){
		if(tok->getType() == "Sym"){
			cout << ((RegularSymbols*)tok)->getSymbol() << " ";
		}else{
			if(((RegularTransition*)tok)->getAvailableTransitions().size() > 0){
				cout << (((RegularTransition*)tok)->getAvailableTransitions())[0] << " ";
			}else{
				if(((RegularTransition*)tok)->isEpsilonAvailable()){
					cout << "&epsilon&" << " ";
				}
			}
		}
	}
	
	cout << endl << endl;
	
	for(RegularExpressionTokens* tok : defintnionOptimized.getDefinition()){
		if(tok->getType() == "Sym"){
			cout << ((RegularSymbols*)tok)->getSymbol() << " ";
		}else{
			if(((RegularTransition*)tok)->getAvailableTransitions().size() == 1){
				cout << (((RegularTransition*)tok)->getAvailableTransitions())[0] << " ";
			}else if(((RegularTransition*)tok)->getAvailableTransitions().size() > 1){
				cout << "[ ";
				for(char x : ((RegularTransition*)tok)->getAvailableTransitions()){
					cout << x << " ";
				}
				cout << "]";
			}else{
				if(((RegularTransition*)tok)->isEpsilonAvailable()){
					cout << "&epsilon&" << " ";
				}
			}
		}
	}


	cout << endl << defintnion.getDefinition().size() << endl << endl;
}
