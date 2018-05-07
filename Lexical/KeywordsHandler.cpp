#include <iostream>
#include <vector>
#include <unordered_set>
#include "../Helper/ErrorReport.h"
#include "RuleTokenize.h"
#include "RegularExpTokens.h"
#include "DefinitionsHandler.h"
#include "Regulars.h"
#include "KeywordsHandler.h"

using namespace std;

void KeywordsHandler::parse(string keyword, unsigned int priority){

	RegularExpression regex;
	
	for(int i=0;i<keyword.length();i++){
		if(i > 1){
			regex.addToken(new RegularSymbols('!'));
		}
		RegularTransition* x = new RegularTransition();
		x->setTransitionName(string()+keyword[i]);
		x->addTransition(keyword[i]);
		regex.addToken(x);
	}
	regex.addToken(new RegularSymbols('!'));

	regex.setName(keyword);
	regex.setPriority(priority);

	//debug(regex);
	defindedKeywords.insert(keyword);
	expressionsVector.push_back(regex);
}

void KeywordsHandler::debug(RegularExpression regex){
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

bool KeywordsHandler::setKeywords(vector<RuleToken*> lineTokens, unsigned int priority){

	//check that the line has at least 2 tokens
	if(lineTokens.size() < 2){
		return false;
	}

	//first token must be "["
	if(lineTokens[0]->getType() != '{'){
		return false;
	}

	//last token must be "]"
	if(lineTokens[lineTokens.size()-1]->getType() != '}'){
		return false;
	}

	//get the keywords token
	for(int i=1;i<lineTokens.size()-1;i++){

		//if the lang keywords are not letters, return error
		if(lineTokens[i]->getType() != RULE_TOKEN_LETTERS){
			error.setError("Language Keywords must be letters");
			return false;
		}

		//if the keyword already defined, make error
		if(defindedKeywords.find(lineTokens[i]->getData()) != defindedKeywords.end()){
			error.setError("dublicate in keywords");
			return false;
		}

		//else parse the keyword
		parse(lineTokens[i]->getData(), priority);
	}

	return true;
}

std::vector<RegularExpression> KeywordsHandler::getAllKeywordsExpressions(){
	return expressionsVector;
}

std::string KeywordsHandler::getError(){
	return error.getError();
}

bool KeywordsHandler::isError(){
	return error.isError();
}
