#include <iostream>
#include <vector>
#include <unordered_map>
#include "../Helper/ErrorReport.h"
#include "RuleTokenize.h"
#include "RegularExpTokens.h"
#include "Regulars.h"
#include "DefinitionsHandler.h"
#include "ExpressionsHandler.h"

using namespace std;


bool ExpressionsHandler::setExpression(std::vector<RuleToken*> lineTokens,
 std::unordered_map<std::string,Regular>& definitionsTools, unsigned int priority){
	
	//check that the line has at least 3 tokens
	if(lineTokens.size() < 3){
		return false;
	}

	//first token must be consist of letters
	if(lineTokens[0]->getType() != RULE_TOKEN_LETTERS){
		return false;
	}

	//second token must be ": sign"
	if(lineTokens[1]->getType() != ':'){
		return false;
	}

	//check for dublicate in the name
	if(expressionsMap.find(lineTokens[0]->getData()) != expressionsMap.end()){
		error.setError("dublicate in Expression name");
		return false;
	}

	//convert to regular definiton
	RegularGenerator generator(lineTokens,definitionsTools,false);
	if(!generator.generateRegular()){
		error.setError(generator.getError());
		return false;
	}

	//get the redular expression and set the name
	Regular x = (generator.getRegular());
	RegularExpression* regExp = (RegularExpression*) (&x);
	regExp->setName(lineTokens[0]->getData());
	regExp->setPriority(priority);

	//add to the map and the vector
	expressionsMap[lineTokens[0]->getData()] = *regExp;
	expressionsVector.push_back(*regExp);
	return true;
}

std::vector<RegularExpression> ExpressionsHandler::getAllExpressions(){
	return expressionsVector;
}

std::string ExpressionsHandler::getError(){
	return error.getError();
}

bool ExpressionsHandler::isError(){
	return error.isError();
}
