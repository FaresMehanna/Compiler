#ifndef RULE_LINE_H
#define RULE_LINE_H

#include "../Helper/StringTokenizer.h"

#include <stdio.h>
#include <string>

/*
 * Class Name : RuleLine
 *
 * This class represent single line from the Rules file.
 */
class RuleLine : public Line{
	
	private:
		
		// The text of the string
		std::string lineText;

		// The line number of the string
		int lineNumber;

		// The tokens of the string
		vector<StringToken> lineTokens;

	public:

		// Construct the base class & set the line and it's number.
		RuleLine(std::string lineText, int lineNumber);

		// Get the number of the line inside the file.
		int getLineNumber() override;

		// Get the string of the line inside the file.
		std::string getLineText() override;

		// Get the tokens of the rule line
		vector<StringToken> getLineToken();

		// set the tokens of the rule line
		void setLineToken(vector<StringToken> data);

};

#endif