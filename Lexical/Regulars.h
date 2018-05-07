#ifndef REGULARS_H
#define REGULARS_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "../Helper/ErrorReport.h"
#include "RuleTokenize.h"
#include "RegularExpTokens.h"

class Regular{
	private:
		std::vector<RegularExpressionTokens*> exp;
		std::string name;
		unsigned int priority;
	public:
		void addToken(RegularExpressionTokens* token);
		std::vector<RegularExpressionTokens*> getDefinition();
		void setName(std::string name);
		std::string getName();
		unsigned int getPriority();
		void setPriority(unsigned int priority);
};

class RegularDefinition : public Regular{

};

class RegularExpression : public Regular{

};


class RegularGenerator{
	private:
		Regular defintnion;
		Regular defintnionPostFix;	//same objects as the above but with different order
		Regular defintnionOptimized;

		bool addToDefinitions;
		std::unordered_map<std::string,Regular>& definitions;
		std::vector<RuleToken*> lineTokens;
		ErrorReport error;

		bool firstPhase();
		bool secondPhase();
		bool thirdPhase();
		int symbolPriority(char);
		void debugInfo();
	public:
		RegularGenerator() = delete;
		RegularGenerator(std::vector<RuleToken*> lineTokens, std::unordered_map<std::string,Regular>& definitions, bool addToDefinitionTools);
		bool generateRegular();
		Regular getRegular();
		bool isError();
		std::string getError();
};


#endif