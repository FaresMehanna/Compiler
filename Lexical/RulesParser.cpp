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
#include "RulesParser.h"
#include "RegularExpTokens.h"

#include "DefinitionsHandler.h"
#include "KeywordsHandler.h"
#include "ExpressionsHandler.h"
#include "PunctuationHandler.h"



using namespace std;

bool RulesParser::setFile(string fileName){
	
	//try to open the file
	RulesFile  = fopen(fileName.c_str(),"r");
	fileEnd = false;

	//if failed, set the error message and return false
	if(RulesFile == NULL){
		error.setError("Can't open the Rules file for reading.");
		return false;
	}

	return true;
}

string RulesParser::getError(){
	return error.getError();
}

string RulesParser::readNextLine(FILE* file){

	string line;
	char c;
    
    // iteratively get characters from standard input, checking for CR (Mac OS), LF (Linux), and CRLF (Windows)
    while ((c = fgetc(file)) != '\r' && c != '\n' && c != EOF){
    	line += c;
    }

    if(c == EOF){
    	fileEnd = true;
    }

    return line;
}


bool RulesParser::parseRules(){

	int counter = 5;

	while(!fileEnd){

		//read next line in the file
		string line = readNextLine(RulesFile);

		//divide the string into several token using hardcoded lexical specifications
		vector<RuleToken*> tokens = tokenizer.tokenize(line);

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

vector<RegularExpression> RulesParser::getParsedExpressions(){
	return AllExps;
}
