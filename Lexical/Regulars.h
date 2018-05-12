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
	
		// To implement deepCopy
		RegularExpression(const RegularExpression& lhs);
		RegularExpression& operator= (const RegularExpression& lhs);
		
		// Construct RegularExpression from RuleFileProductionRegularExpression
		RegularExpression(const RuleFileProductionRegularExpression& lhs);
		RegularExpression& operator= (const RuleFileProductionRegularExpression& lhs);

		// Construct RegularExpression from RuleFileUserDefinedRegularExpression
		RegularExpression(const RuleFileUserDefinedRegularExpression& lhs);
		RegularExpression& operator= (const RuleFileUserDefinedRegularExpression& lhs);

		// To Free the data
		virtual ~RegularExpression();

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

		// Construct RuleFileUserDefinedRegularExpression from RegularExpression
		RuleFileUserDefinedRegularExpression(const RegularExpression& lhs);
		RuleFileUserDefinedRegularExpression& operator= (const RegularExpression& lhs);
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

#endif