#ifndef FILE_READER_H
#define FILE_READER_H

#include "File.h"
#include <string>

class FileReader : public File{

	public:

		FileReader();
		bool setFile(std::string fileName);
		
		std::string getNextLine();
		bool isEnd();

		bool isError();
		std::string getError();
};


#endif