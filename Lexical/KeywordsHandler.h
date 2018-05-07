#ifndef KEYWORDS_HANDLER_H
#define KEYWORDS_HANDLER_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include "../Helper/ErrorReport.h"
#include "RuleTokenize.h"
#include "RegularExpTokens.h"
#include "DefinitionsHandler.h"
#include "Regulars.h"


class KeywordsHandler{
	
	private:
		ErrorReport error;
		std::unordered_set<std::string> defindedKeywords;
		std::vector<RegularExpression> expressionsVector;

		void parse(std::string keyword, unsigned int priority);
		void debug(RegularExpression regex);
	public:
		bool setKeywords(std::vector<RuleToken*> lineTokens, unsigned int priority);
		std::vector<RegularExpression> getAllKeywordsExpressions();
		std::string getError();
		bool isError();
};

#endif