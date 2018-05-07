#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "../Helper/ErrorReport.h"
#include "RuleTokenize.h"

using namespace std;

RuleToken::RuleToken(int type ,const string& data){
	this->type = type;
	this->data = data;
}

int RuleToken::getType(){
	return type;
}

string RuleToken::getData(){
	return data;
}


string RuleTokenizer::getError(){
	return error.getError();
}

bool RuleTokenizer::isError(){
	return error.isError();
}

vector<RuleToken*> RuleTokenizer::tokenize(string str){
	
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
	vector<RuleToken*> toReturn;

	/* 
	 * Token parsing.
	 */

	for(char x : str){

		// Did the curr char match any of the know token classes?
		// If no, error will be reported.
		bool found = false;

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
			found = true;
			//add to curr string
			currString += currChar;	
			//set oldtype = type then type=RULE_TOKEN_LETTERS
			swap(type,oldtype);
			type = RULE_TOKEN_LETTERS;
		}
		if(currChar >= 'A' && currChar <= 'Z'){
			found = true;
			//add to curr string
			currString += currChar;	
			//set oldtype = type then type=RULE_TOKEN_LETTERS
			swap(type,oldtype);
			type = RULE_TOKEN_LETTERS;
		}

		// Type2 = numbers.
		if(currChar >= '0' && currChar <= '9'){
			found = true;
			//add to curr string
			currString += currChar;	
			//set oldtype = type then type=RULE_TOKEN_DIGITS
			swap(type,oldtype);
			type = RULE_TOKEN_DIGITS;
		}

		// Handle escape chars.
		if(currChar == '\\'){
			found = true;

			// Process the following char.
			if(str.length() > counter+1){

				// Check and add the escape char.
				// TODO
				currString += str[counter+1];

				// If it was lamda, add = to identify it.
				// TODO
				if(str[counter+1] == 'L'){
				currString += '=';
				}

				// Set escape flag = true, to skip the next char
				//as we already processed it.
				escp = true;

			// If it is the last char, report the error and return empty vec.
			}else{
				error.setError("Escape char can't be the last char in rule sentence.");
				return {};
			}

			// Set oldtype = type then type=RULE_TOKEN_SYMBOLS.
			swap(type,oldtype);
			type = RULE_TOKEN_SYMBOLS;
		}

		// Symbols.
		if(currChar == '/' || currChar == '!' ||
		   currChar == '<' || currChar == '>' ){
			found = true;

			// Add to curr string.
			currString += currChar;

			// Set oldtype = type then type=RULE_TOKEN_SYMBOLS.
			swap(type,oldtype);
			type = RULE_TOKEN_SYMBOLS;
		}

		// Type-generic = symbol.
		if(currChar == '|' || currChar == '-' ||
		   currChar == '+' || currChar == '*' ||
		   currChar == '(' || currChar == ')' ||
		   currChar == '{' || currChar == '}' ||
		   currChar == '=' || currChar == ':' ||
		   currChar == ';' || currChar == ',' ||
		   currChar == '[' || currChar == ']' ||
		   currChar == '.' || currChar == '\''||
		   currChar == '#'){
			found = true;

			// Add to curr string.
			currString += currChar;	
		
			// Set oldtype = type then type=generic.
			swap(type,oldtype);
			type = currChar;
		}

		// Type = -1.
		// Space or tab.
		if(currChar == ' ' || currChar == '\t'){
			found = true;
			// Add to curr string.
			swap(type,oldtype);
			type = -1;
		}

		// If this char didn't match any of the token classes.
		// Report error and return empty vec.
		if(!found){
			error.setError("Unrecognized char during parsing the rules file.");
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
				if(oldtype == RULE_TOKEN_LETTERS || oldtype == RULE_TOKEN_DIGITS || oldtype == RULE_TOKEN_SYMBOLS){
					RuleToken* token = new RuleToken(oldtype,currString);
					toReturn.push_back(token);

				// If they are symbols then add them as seperate strings.
				}else{
					for(int i=0;i<currString.length();i++){
						RuleToken* token = new RuleToken(oldtype,string()+currString[i]);
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
				if(oldtype == RULE_TOKEN_LETTERS || oldtype == RULE_TOKEN_DIGITS || oldtype == RULE_TOKEN_SYMBOLS){
					RuleToken* token = new RuleToken(oldtype,string(currString.begin(),currString.end()-1));
					toReturn.push_back(token);
				//if they are symbols then add them as seperate strings
				}else{
					for(int i=0;i<currString.length()-1;i++){
						RuleToken* token = new RuleToken(oldtype,string()+currString[i]);
						toReturn.push_back(token);
					}
				}

				currString = currString[currString.length()-1];
			}
		}
	}

	if(currString.length() > 0){
		RuleToken* token = new RuleToken(type,string(currString.begin(),currString.end()));
		toReturn.push_back(token);
	}

	return toReturn;
}