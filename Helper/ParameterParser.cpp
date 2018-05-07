#include <string>
#include <iostream>
#include "ErrorReport.h"
#include "ParameterParser.h"

using namespace std;

std::string ParameterParser::get(int numOfArguments,const char** array, string startingPattern){
	
	bool found = false;

	for(int i=0;i<numOfArguments;i++){
		
		int len = strlen(array[i]);
		if(len > 3){
		
			if(memcmp(array[i],startingPattern.c_str(),3) == 0){
				return string(array[i]+3);
			}
		
		}

	}
	
	error.setError("YOU MUST RUN THE COMPILER WITH ./compiler -l=lexicalFileName -c=codeFileName -p=ProductionFileName");
	return "";
}

string ParameterParser::getLexicalRulesFileName(int numOfArguments,const char** array){
	return get(numOfArguments,array,"-l=");
}

string ParameterParser::getCodeFileName(int numOfArguments,const char** array){
	return get(numOfArguments,array,"-c=");
}

string ParameterParser::getProductionRulesFileName(int numOfArguments,const char** array){
	return get(numOfArguments,array,"-p=");
}

bool ParameterParser::isError(){
	return error.isError();
}

std::string ParameterParser::getError(){
	return error.getError();
}
