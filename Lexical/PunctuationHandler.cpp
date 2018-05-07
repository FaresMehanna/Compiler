#include <iostream>
#include <vector>
#include <unordered_set>
#include "../Helper/ErrorReport.h"
#include "RuleTokenize.h"
#include "RegularExpTokens.h"
#include "DefinitionsHandler.h"
#include "Regulars.h"
#include "PunctuationHandler.h"

using namespace std;

void PunctuationHandler::debug(RegularExpression regex){
	for(RegularExpressionTokens* tok : regex.getDefinition()){
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
}

bool PunctuationHandler::setPunctuations(vector<RuleToken*> lineTokens, unsigned int priority){
	//check that the line has at least 2 tokens
	if(lineTokens.size() < 2){
		return false;
	}

	//first token must be "["
	if(lineTokens[0]->getType() != '['){
		return false;
	}

	//last token must be "]"
	if(lineTokens[lineTokens.size()-1]->getType() != ']'){
		return false;
	}

	//get the keywords token
	for(int i=1;i<lineTokens.size()-1;i++){

		RegularExpression regex;

		//if the lang keywords are not letters, return error
		if(lineTokens[i]->getData().length() != 1){
			error.setError("Error in punctuation length");
			return false;
		}

		RegularTransition* x = new RegularTransition();
		switch((lineTokens[i]->getData())[0]){
			case '(':
			case ')':
			case '{':
			case '}':
			case ';':
			case ',':
				x->addTransition((lineTokens[i]->getData())[0]);
				x->setTransitionName(string()+(lineTokens[i]->getData())[0]);
				regex.addToken(x);
			break;
			default:
				error.setError("Error in punctuation symbol");
				return false;
			break;
		}

		regex.setName(lineTokens[i]->getData());
		regex.setPriority(priority);

		expressionsVector.push_back(regex);
	}

	return true;

}

vector<RegularExpression> PunctuationHandler::getAllPunctuationsExpressions(){
	return expressionsVector;
}

string PunctuationHandler::getError(){
	return error.getError();
}
bool PunctuationHandler::isError(){
	return error.isError();
}
