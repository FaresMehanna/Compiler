#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "ErrorReport.h"
#include "StringTokenizer.h"

using namespace std;

StringToken::StringToken(int type ,const string& data){
	this->type = type;
	this->data = data;
}

int StringToken::getType(){
	return type;
}

string StringToken::getData(){
	return data;
}



string StringTokenizer::getError(){
	return error.getError();
}

bool StringTokenizer::isError(){
	return error.isError();
}

vector<StringToken*> StringTokenizer::tokenize(const std::string& str){
	
	// if empty or NULL string, return empty vector.
	if(str.length() == 0)
		return {};

	/* 
	 * necessary information to do the parsing.
	 */

	// type of the current char and type of the prev char, this can
	//help to determine when single token starts and end.
	int type = -1;
	int oldtype = -1;

	// keep track of what is the current index for the current char.
	int counter = -1;

	// if there was escape charachter, this will tell the tokenizer to
	//add the next charachter directly without "Escape chars" checking.
	bool escp = false;

	// current Token, in each char we decide if it's belong to the curr Token, or
	//should start new Token and save the old one.
	string currString = "";

	// vector of tokens to be returned.
	vector<StringToken*> toReturn;

	/* 
	 * Token parsing.
	 */

	for(char x : str){

		// better Name? :D
		char currChar = x;

		// counter inc.
		counter++;

		// if escp is true then skip this char, since this char already processed
		//in the prev iteration.
		if(escp){
			escp = false;
			continue;
		}

		// Type1 = strings.
		if(currChar >= 'a' && currChar <= 'z'){
			//add to curr string
			currString += currChar;	
			//set oldtype = type then type=STRING_TOKEN_LETTERS
			swap(type,oldtype);
			type = STRING_TOKEN_LETTERS;
		}

		else if(currChar >= 'A' && currChar <= 'Z'){
			//add to curr string
			currString += currChar;	
			//set oldtype = type then type=STRING_TOKEN_LETTERS
			swap(type,oldtype);
			type = STRING_TOKEN_LETTERS;
		}

		// Type2 = numbers.
		else if(currChar >= '0' && currChar <= '9'){
			//add to curr string
			currString += currChar;	
			//set oldtype = type then type=STRING_TOKEN_DIGITS
			swap(type,oldtype);
			type = STRING_TOKEN_DIGITS;
		}


		// Handle escape chars.
		else if(currChar == '\\'){

			// Process the following char.
			if(str.length() > counter+1){

				// Check and add the escape char.
				if(str[counter+1] != '-' && str[counter+1] != '+' &&
				   str[counter+1] != '|' && str[counter+1] != '*' &&
				   str[counter+1] != '(' && str[counter+1] != ')' &&
				   str[counter+1] != 'L'){
					error.setError("Escape char not recognized.");
					return {};
				}
				currString += str[counter+1];

				// Set escape flag = true, to skip the next char
				//as we already processed it.
				escp = true;

			// If it is the last char, report the error and return empty vec.
			}else{
				error.setError("Escape char can't be the last char in the string.");
				return {};
			}

			// Set oldtype = type then type=STRING_TOKEN_ESCAPE.
			swap(type,oldtype);
			type = STRING_TOKEN_ESCAPE;
		}

		// Type-generic = symbol.
		else if(currChar == '|' || currChar == '-' ||
			    currChar == '+' || currChar == '*' ||
			    currChar == '(' || currChar == ')' ||
			    currChar == '{' || currChar == '}' ||
			    currChar == '=' || currChar == ':' ||
 			    currChar == ';' || currChar == ',' ||
 			    currChar == '[' || currChar == ']' ||
 			    currChar == '.' || currChar == '\''||
 			    currChar == '#' || currChar == '/' ||
 			    currChar == '<' || currChar == '>' ||
 			    currChar == '!' || currChar == '_' ||
 			    currChar == '$' || currChar == '%' ||
 			    currChar == '~' || currChar == '@' ||
 			    currChar == '&' || currChar == '^'){

			// Add to curr string.
			currString += currChar;	
		
			// Set oldtype = type then type=generic.
			swap(type,oldtype);
			type = STRING_TOKEN_SYMBOL;
		}

		// Type = -1.
		// Space or tab.
		else if(currChar == ' ' || currChar == '\t'){
			// Add to curr string.
			swap(type,oldtype);
			type = -1;
		}

		// Did the curr char match any of the know token classes?
		// If no, error will be reported and the operation will be apported.
		else{
			error.setError("Unrecognized char during parsing the string.");
			return {};
		}

		// If there is transfer between types and there is actual string.
		if(type != oldtype && currString != ""){

			// If now is space and comming from other type, that's mean
			//the end of a token.
			// Add the string to the vector.
			if(type == -1){

				// If it's letters or digits or operators,
				//add them as one string.
				if(oldtype == STRING_TOKEN_LETTERS || oldtype == STRING_TOKEN_DIGITS || oldtype == STRING_TOKEN_ESCAPE){
					StringToken* token = new StringToken(oldtype,currString);
					toReturn.push_back(token);

				// If they are symbols then add them as seperate strings.
				}else{
					for(int i=0;i<currString.length();i++){
						StringToken* token = new StringToken(oldtype,string()+currString[i]);
						toReturn.push_back(token);
					}
				}

				// Start and new Token.
				currString = "";

			// If last one was space that's mean that the this is the first letter,
			//so skip it because it might be followed by the letter of the same type.
			}else if(oldtype == -1){
				continue;
			
			//else that mean you converted from type to another type but both are not spaces
			//so add the last string and start to build the new one
			}else{

				//if it's letters or digits or operators //add them as one string
				if(oldtype == STRING_TOKEN_LETTERS || oldtype == STRING_TOKEN_DIGITS || oldtype == STRING_TOKEN_ESCAPE){
					StringToken* token = new StringToken(oldtype,string(currString.begin(),currString.end()-1));
					toReturn.push_back(token);
				//if they are symbols then add them as seperate strings
				}else{
					for(int i=0;i<currString.length()-1;i++){
						StringToken* token = new StringToken(oldtype,string()+currString[i]);
						toReturn.push_back(token);
					}
				}

				currString = currString[currString.length()-1];
			}
		}
	}

	if(currString.length() > 0){
		//if it's letters or digits or operators //add them as one string
		if(type == STRING_TOKEN_LETTERS || type == STRING_TOKEN_DIGITS || type == STRING_TOKEN_ESCAPE){
			StringToken* token = new StringToken(type,string(currString.begin(),currString.end()));
			toReturn.push_back(token);
		//if they are symbols then add them as seperate strings
		}else{
			for(int i=0;i<currString.length();i++){
				StringToken* token = new StringToken(type,string()+currString[i]);
				toReturn.push_back(token);
			}
		}
	}

	return toReturn;
}