#ifndef PARAMETER_PARSER_H
#define PARAMETER_PARSER_H

#include <string>
#include "ErrorReport.h"

class ParameterParser{
	
	private:
		ErrorReport error;

	public:
		
		std::string get(int numOfArguments,const char** array, std::string startingPattern);

		std::string getLexicalRulesFileName(int numOfArguments,const char** array);
		std::string getCodeFileName(int numOfArguments,const char** array);
		std::string getProductionRulesFileName(int numOfArguments,const char** array);

		bool isError();
		std::string getError();
};

#endif