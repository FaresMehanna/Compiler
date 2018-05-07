#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include "../Helper/ErrorReport.h"
#include "RuleTokenize.h"
#include "DefinitionsHandler.h"
#include "RegularExpTokens.h"
#include "Regulars.h"
#include <deque>

using namespace std;


bool DefinitionsHandler::setDefinition(vector<RuleToken*> lineTokens, unsigned int priority){
	
	//check that the line has at least 3 tokens
	if(lineTokens.size() < 3){
		return false;
	}

	//first token must be consist of letters
	if(lineTokens[0]->getType() != RULE_TOKEN_LETTERS){
		return false;
	}

	//second token must be "equal sign"
	if(lineTokens[1]->getType() != '='){
		return false;
	}

	//check for dublicate in the name
	if(definitions.find(lineTokens[0]->getData()) != definitions.end()){
		error.setError("dublicate in Definition name");
		return false;
	}

	//convert to regular definiton
	RegularGenerator generator(lineTokens,definitionsTools,true);
	if(!generator.generateRegular()){
		error.setError(generator.getError());
		return false;
	}

	definitions[lineTokens[0]->getData()] = generator.getRegular();
	return true;
}

std::unordered_map<std::string,Regular>& DefinitionsHandler::getDefinitionsTools(){
	return definitionsTools;
}


bool DefinitionsHandler::isDefinitionAvailable(string defName){
	return definitions.find(defName) != definitions.end();
}

RegularDefinition DefinitionsHandler::getDefinition(string defName){
	return *(RegularDefinition*) &(definitions[defName]);
}

string DefinitionsHandler::getError(){
	return error.getError();
}

bool DefinitionsHandler::isError(){
	return error.isError();
}


