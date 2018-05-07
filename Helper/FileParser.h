#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <string>
#include "ErrorReport.h"
#include "FileReader.h"

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