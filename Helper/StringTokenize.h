#ifndef STRING_TOKENIZE_H
#define STRING_TOKENIZE_H

#include "ErrorReport.h"
#include <vector>
#include <string>


/*
 * Definitions : String Types
 *
 * THIS LIST DO NOT CONTAIN ALL THE TYPES
 */
#define STRING_TOKEN_LETTERS	1
#define STRING_TOKEN_DIGITS		2
#define STRING_TOKEN_ESCAPE		3
#define STRING_TOKEN_SYMBOL		4

/*
 * Class Name : StringToken
 *
 * This class represent single token in the string.
 * Storing only the token itself and it's type.
 */
class StringToken{

		// the type of this token.
		// the list of the types are declared above.
		int type;

		// the data included in the token.
		std::string data;
	
	public:

		// the default constructor needs the type and the data to construct the token.
		StringToken(int type ,const std::string& data);

		// getter functions to get the data represented in the StringToken.
		int getType();
		std::string getData();
};

/*
 * Class Name : StringTokenizer
 *
 * This class accept a string and try to tokenize it.
 * The string must comply with the "Tokenizer" rules. If error occured the isError()
   will return true and the reason on the getError().
 */
class StringTokenizer{

		// private instance from error object to control errors.
		ErrorReport error;

	public:

		// accept a string and try to tokenize it.
		// in case of succ, vector of tokens will be returned and isError() will
		//return false.
		// incase of failure, empty vector will be returned and isError() will
		//return true and getError() will return the reason.
		std::vector<StringToken*> tokenize(const std::string& str);

		// isError() functions to ask if there is an error.
		// if there was getError() will return the reason.
		bool isError();
		std::string getError();
};

#endif