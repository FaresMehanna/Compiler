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

		// Path & Name of the file.
		std::string FileName;

		// Object to report error to the user.
		ErrorReport error;

		// File handler to read the file to be parsed.
		FileReader reader;
	
	public:

		// Set the full path&name to the rule file. will return true incase of found the file and 
		//opened successfully, Or it will return false if error happens and getError() will return
		//the exact problem.
		bool setFile(const std::string& fileName);
		
		// This function will be overriden in every child class to implement it's function.
		virtual bool parse() = 0;

		// Functions to get test and get the errors.
		bool isError();
		std::string getError();
	
};

#endif