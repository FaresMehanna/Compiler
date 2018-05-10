#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <limits.h>
#include "../Helper/ErrorReport.h"
#include "RuleTokenize.h"
#include "RulesFileParser.h"
#include "RegularExpTokens.h"

#include "DefinitionsHandler.h"
#include "KeywordsHandler.h"
#include "ExpressionsHandler.h"
#include "PunctuationHandler.h"



using namespace std;

bool RulesFileParser::setFile(string fileName){
	
	//try to open the file
	bool succ = ruleFileReader.setFile(fileName));

	//if failed, set the error message and return false
	if(!succ){
		error.setError(ruleFileReader.getError());
		return false;
	}

	return true;
}

string RulesFileParser::getError(){
	return error.getError();
}


bool RulesFileParser::parseRules(){

	// This is increasing counter for every rule.
	// This counter will be used to set priority for each line, priority is bigger
	//at the top and decrease downward. So priority will be (INT_MAX/counter).
	int counter = 5;

	while(!ruleFileReader.isEnd()){

		//read next line in the file
		string line = ruleFileReader.getNextLine();

		//divide the string into several token using hardcoded lexical specifications
		vector<StringToken*> tokens = tokenizer.tokenize(line);

		//check error during the tokenizer
		if(tokenizer.isError()){
			error.setError(tokenizer.getError());
			return false;
		}

		//Solve the line
		bool succ = true;

		//try to solve the line as definition
		succ |= defsHandler.setDefinition(tokens,INT_MAX);
		//try to solve the line as expression
		succ |= expHandler.setExpression(tokens,defsHandler.getDefinitionsTools(),INT_MAX/counter);
		//try to solve the line as keywords
		succ |= keyHandler.setKeywords(tokens,INT_MAX/2);
		//try to solve the line as punctation
		succ |= punctHandler.setPunctuations(tokens,INT_MAX/3);

		//if there was an error
		//set the error message and return false
		if(!succ){
			if(defsHandler.isError()){
				error.setError(defsHandler.getError());
			}else if(expHandler.isError()){
				error.setError(expHandler.getError());
			}else if(keyHandler.isError()){
				error.setError(keyHandler.getError());
			}else if(punctHandler.isError()){
				error.setError(punctHandler.getError());
			}else{
				error.setError("Given rule isn't definition nor expression nor keyword nor punctation.");
			}
			return false;
		}
		counter++;
	}

	//All expressions
	vector<RegularExpression> exps = expHandler.getAllExpressions();
	//All keywords as expressions
	vector<RegularExpression> keyExps = keyHandler.getAllKeywordsExpressions();
	//All punctuations as expressions 
	vector<RegularExpression> punctuationExps = punctHandler.getAllPunctuationsExpressions();

	//add them together
	AllExps.clear();	//clear old data
	AllExps.insert(AllExps.end(), keyExps.begin(), keyExps.end());
	AllExps.insert(AllExps.end(), punctuationExps.begin(), punctuationExps.end());
	AllExps.insert(AllExps.end(), exps.begin(), exps.end());

	return true;

}

vector<RegularExpression> RulesFileParser::getParsedExpressions(){
	return AllExps;
}
