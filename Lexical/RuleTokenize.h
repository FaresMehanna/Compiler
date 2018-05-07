#ifndef RULE_TOKENIZE_H
#define RULE_TOKENIZE_H

#include "../Helper/ErrorReport.h"
#include <vector>
#include <string>


/*
 * Definitions : Rule Types
 *
 * THIS LIST DO NOT CONTAIN ALL THE TYPES
 */
#define RULE_TOKEN_LETTERS	1
#define RULE_TOKEN_DIGITS	2
#define RULE_TOKEN_SYMBOLS	3


/*
 * Class Name : RuleToken
 *
 * This class represent single token in the rule files.
 * Storing only the token itself and it's type.
 */
class RuleToken{

		// the type of this token.
		// the list of the types are declared above.
		int type;

		// the data included in the token.
		std::string data;
	
	public:

		// the default constructor needs the type and the data to construct the token.
		RuleToken(int type ,const std::string& data);

		// getter functions to get the data represented in the RuleToken.
		int getType();
		std::string getData();
};

/*
 * Class Name : RuleTokenizer
 *
 * This class accept a string and try to tokenize it. This string should represent a
   single line from the rules file.
 * The string must comply with the "Rule File" rules. If error occured the isError()
   will return true and the reason on the getError().
 */
class RuleTokenizer{

		// private instance from error object to control errors.
		ErrorReport error;

	public:

		// accept a string and try to tokenize it.
		// incase of succ, vector of tokens will be returned and isError() will
		//return false.
		// incase of failure, empty vector will be returned and isError() will
		//return true and getError() will return the reason.
		std::vector<RuleToken*> tokenize(std::string str);

		// isError() functions to ask if there is an error.
		// if there was getError() will return the reason.
		bool isError();
		std::string getError();
};

#endif