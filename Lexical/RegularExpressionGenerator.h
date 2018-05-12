#ifndef REGULAR_EXPRESSION_GENERATOR_H
#define REGULAR_EXPRESSION_GENERATOR_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "../Helper/ErrorReport.h"
#include "../Helper/StringTokenizer.h"

#include "Regulars.h"

class RegularExpressionGenerator{
	
	private:

		// Regular Expressions to convert a regular expression to an optimized one.
		// these objects will store the intermediate representations of the regular expression.
		RegularExpression defintnion;			// Original Regular expression from the string tokens.
		RegularExpression defintnionPostFix;	// Same objects as the above but with different order.
		RegularExpression defintnionOptimized;	// Obtimized version.

		// This is hashmap to store the user defined regular to be used in other regular expressions.
		std::unordered_map<std::string,RuleFileUserDefinedRegularExpression>& userDefinedRegulars;
		
		// To report an error.
		ErrorReport error;

		// Three phases to convert a line consisting of some (StringToken)s to post-fix optimized regular
		//Expression.
		// Will return true incase of succ and false otherwise.
		bool firstPhase(const vector<StringToken>& lineTokens);		// Convert the Tokens to basic RegularExpression.
		bool secondPhase();		// Convert the Tokens to postFix order
		bool thirdPhase();		// Optimize the postFix ordering.
		
		// This function will be used to convert the regular expression to post-fix, this function
		//will return the priority of every operation.
		int symbolPriority(char);

	public:

		// "definitions": This is hashmap to store the user defined regular to be used in other regular expressions.
		RegularExpressionGenerator(std::unordered_map<std::string,RuleFileUserDefinedRegularExpression>& definitions);
		
		// Try to generat the Regular expression from the vector of Tokens.
		// "isUserDefinedRegulars": Is this Regular expression a user defined on to be used for other regular expressions and won't be in production?
		// "lineTokens": This is refrence of vector of The tokens in the rule line.
		bool generateRegularExpression(const std::vector<StringToken>& lineTokens, bool isUserDefinedRegulars = false);
		
		// In case succ of "generateRegular", this function will return optimized post-fix regularExpression.
		RegularExpression getProductionRegularExpression();
		
		// Functions to report the error to the user.
		bool isError();
		std::string getError();
};

#endif