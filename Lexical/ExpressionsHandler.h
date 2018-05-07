#ifndef EXPRESSIONS_HANDLER_H
#define EXPRESSIONS_HANDLER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "../Helper/ErrorReport.h"
#include "RuleTokenize.h"
#include "RegularExpTokens.h"
#include "DefinitionsHandler.h"
#include "Regulars.h"


class ExpressionsHandler{
	
	private:
		ErrorReport error;
		std::unordered_map<std::string,RegularExpression> expressionsMap;
		std::vector<RegularExpression> expressionsVector;

	public:
		bool setExpression(std::vector<RuleToken*> lineTokens,
		std::unordered_map<std::string,Regular>& definitionsTools, unsigned int priority);
		std::vector<RegularExpression> getAllExpressions();
		std::string getError();
		bool isError();
};

#endif