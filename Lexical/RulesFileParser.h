#ifndef RULES_FILE_PARSER_H
#define RULES_FILE_PARSER_H

#include "../Helper/ErrorReport.h"
#include "../Helper/FileReader.h"
#include "../Helper/StringTokenizer.h"
#include "../Helper/FileParser.h"

#include "DefinitionsHandler.h"
#include "ExpressionsHandler.h"
#include "KeywordsHandler.h"
#include "PunctuationHandler.h"

#include <stdio.h>
#include <string>

/*
 * Class Name : RulesFileParser
 *
 * Given the file path and name, this class will parse the rules file and will return
   all the rules from this file as RegularExpression objects.
 */
class RulesFileParser : public FileParser{
	
	private:
		
		// Object from FileReader to handle the file reading.
		FileReader ruleFileReader;

		//Object from StringTokenizer to tokenize each rule line to several tokens.
		StringTokenizer tokenizer;

		// All expressions to be returned to the user.
		std::vector<RuleFileProductionRegularExpression> AllExps;

		/* Handlers to different king of lines that can be presented in the rules file. */

		// Expressions in This handler will only be used by the user and
		//(ExpressionsHandler | KeywordsHandler | PunctuationHandler) and will not be returned
		//to the user.
		DefinitionsHandler defsHandler;

		// Expressions in Those handlers will be returned to the user.
		ExpressionsHandler expHandler;
		KeywordsHandler keyHandler;
		PunctuationHandler punctHandler;
		
	public:

		// Construct the base class.
		RulesFileParser();
		
		// Try to Parse all the rules in the given file, return true if all the rules parsed correctly,
		//flase otherwise.
		bool parse() override;

		// If parsing done successfully, you may call getParsedExpressions() to return all the needed
		//expressions from (ExpressionsHandler | KeywordsHandler | PunctuationHandler).
		std::vector<RuleFileProductionRegularExpression> getParsedExpressions();
};

#endif