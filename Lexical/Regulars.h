#ifndef REGULARS_H
#define REGULARS_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "../Helper/ErrorReport.h"
#include "../Helper/StringTokenizer.h"

#include "RegularExpTokens.h"

/*
 * Class Name : RegularExpression
 *
 * This class represent every full regular expression, which could represent
 * a line from Rules File.
 */

class RegularExpression{

	private:

		// List of the tokens in this regular expression.
		std::vector<RegularExpressionToken*> expressionTokens;
		
		// Name of the regular expression.
		std::string name;
	
	public:
	
		// Add token to then end of the current regular expression.
		void addToken(RegularExpressionToken* token);
		
		// Get the vector of tokens that make the regular expression.
		std::vector<RegularExpressionToken*> getRegularExpression();

		//clean the regular expressions
		virtual void clean();
	
		// Setter and Getter of the regular expression name.
		void setName(std::string name);
		std::string getName();
};

/*
 * Class Name : RuleFileUserDefinedRegularExpression
 *
 * Proxy of "RegularExpression" for lines in the Rules File that user define for further usage.
 */
class RuleFileUserDefinedRegularExpression : public RegularExpression{

};

/*
 * Class Name : RuleFileRegularExpression
 *
 * Rules in the rules file that will be used in the rest of the lexical analyzer.
 */
class RuleFileProductionRegularExpression : public RegularExpression{

	private:
		unsigned int priority;
	
	public:

		RuleFileProductionRegularExpression();

		//clean the data
		void clean() override;

		// Construct RuleFileProductionRegularExpression from RegularExpression
		RuleFileProductionRegularExpression(const RegularExpression& lhs);
		RuleFileProductionRegularExpression& operator= (const RegularExpression& lhs);

		// Setter and Getter for the regular priority.
		unsigned int getPriority();
		void setPriority(unsigned int priority);
};


class RegularExpressionGenerator{
	
	private:

		// Regular Expressions to convert a regular expression to an optimized one.
		// these objects will store the intermediate representations of the regular expression.
		RegularExpression defintnion;			// Original Regular expression from the string tokens.
		RegularExpression defintnionPostFix;	// Same objects as the above but with different order.
		RegularExpression defintnionOptimized;	// Obtimized version.

		
		// Is this Regular expression a user defined on to be used for other regular expressions and won't be in production?
		bool isUserDefinedRegulars;

		// This is hashmap to store the user defined regular to be used in other regular expressions.
		std::unordered_map<std::string,RuleFileUserDefinedRegularExpression>& userDefinedRegulars;

		// This is refrence of vector of The tokens in the rule line.
		const std::vector<StringToken*>& lineTokens;
		
		// To report an error.
		ErrorReport error;

		// Three phases to convert a line consisting of some (StringToken)s to post-fix optimized regular
		//Expression.
		// Will return true incase of succ and false otherwise.
		bool firstPhase();		// Convert the Tokens to basic RegularExpression.
		bool secondPhase();		// Convert the Tokens to postFix order
		bool thirdPhase();		// Optimize the postFix ordering.
		
		// This function will be used to convert the regular expression to post-fix, this function
		//will return the priority of every operation.
		int symbolPriority(char);

	public:

		RegularExpressionGenerator(const std::vector<StringToken*>& lineTokens, std::unordered_map<std::string,RuleFileUserDefinedRegularExpression>& definitions, bool isUserDefinedRegulars);
		
		// Try to generat the Regular expression from the vector of Tokens.
		bool generateRegularExpression();
		
		// In case succ of "generateRegular", this function will return optimized post-fix regularExpression.
		RuleFileProductionRegularExpression getProductionRegularExpression();
		
		// Functions to report the error to the user.
		bool isError();
		std::string getError();
};

#endif