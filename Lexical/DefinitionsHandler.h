#ifndef DEFINITIONS_HANDLER_H
#define DEFINITIONS_HANDLER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "../Helper/ErrorReport.h"
#include "RuleTokenize.h"
#include "RegularExpTokens.h"
#include "Regulars.h"


class DefinitionsHandler{
	
	private:
		ErrorReport error;
		std::unordered_map<std::string,Regular> definitions;
		std::unordered_map<std::string,Regular> definitionsTools;

	public:
		bool setDefinition(std::vector<RuleToken*> lineTokens, unsigned int priority);
		bool isDefinitionAvailable(std::string defName);
		RegularDefinition getDefinition(std::string defName);
		std::unordered_map<std::string,Regular>& getDefinitionsTools();
		std::string getError();
		bool isError();
};

#endif