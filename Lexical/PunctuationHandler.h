#ifndef PUNCTUATION_HANDLER_H
#define PUNCTUATION_HANDLER_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include "../Helper/ErrorReport.h"
#include "RuleTokenize.h"
#include "RegularExpTokens.h"
#include "DefinitionsHandler.h"
#include "Regulars.h"


class PunctuationHandler{
	
	private:
		ErrorReport error;
		std::vector<RegularExpression> expressionsVector;

		void debug(RegularExpression regex);
	public:
		bool setPunctuations(std::vector<RuleToken*> lineTokens, unsigned int priority);
		std::vector<RegularExpression> getAllPunctuationsExpressions();
		std::string getError();
		bool isError();
};

#endif