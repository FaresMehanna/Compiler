#ifndef RULES_PARSER_H
#define RULES_PARSER_H

#include "RuleTokenize.h"
#include "../Helper/ErrorReport.h"

#include "DefinitionsHandler.h"
#include "ExpressionsHandler.h"
#include "KeywordsHandler.h"
#include "PunctuationHandler.h"

#include <stdio.h>
#include <string>

class RulesParser{
	
	private:
		
		FILE* RulesFile = NULL;
		ErrorReport error;
		RuleTokenizer tokenizer;
		bool fileEnd;

		//All expressions to be returned
		std::vector<RegularExpression> AllExps;

		DefinitionsHandler defsHandler;
		ExpressionsHandler expHandler;
		KeywordsHandler keyHandler;
		PunctuationHandler punctHandler;
		
		std::string readNextLine(FILE* file);
	public:
		bool setFile(std::string fileName);
		std::string getError();
		bool parseRules();
		std::vector<RegularExpression> getParsedExpressions();
		//vector<RegularExpression> getExpressions();
};

#endif