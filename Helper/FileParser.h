#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <string>
#include "ErrorReport.h"
#include "FileReader.h"

/*
 * Class Name : FileParser
 *
 * This interface for any class that will handle file like RulesFile or ProductionFile.
 */
class FileParser{
	
	protected:

		std::string FileName;
		ErrorReport error;
		FileReader fr;
	
	public:
		FileParser(const std::string& FileName);
		virtual bool parse() = 0;

		bool isError();
		std::string getError();
	
};

#endif