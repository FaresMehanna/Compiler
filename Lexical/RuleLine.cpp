#ifndef RULE_LINE_H
#define RULE_LINE_H

#include "../Helper/StringTokenizer.h"

#include <stdio.h>
#include <string>

using namespace std;


RuleLine(string lineText, int lineNumber){
	this->lineText = lineText;
	this->lineNumber = lineNumber;
}


int getLineNumber(){
	return lineNumber;
}


string getLineText(){
	return lineText;
}


vector<StringToken> getLineToken(){
	return lineTokens;
}

void setLineToken(vector<StringToken> data){
	lineTokens = data;
}


#endif