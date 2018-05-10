#ifndef FILE_READER_H
#define FILE_READER_H

#include "File.h"
#include <string>

/*
 * Class Name : FileReader
 *
 * This class will handle all reading from files in the compiler.
 */
class FileReader : public File{

	public:

		// It will call the base class constructor.
		FileReader();

		// It will set the file name and try to open the file. if the file exist and opened
		//successfully true will be returned, else it will return false and getError() will return the
		//exact problem.
		bool setFile(std::string fileName);
		
		// It will return the next line in the file.
		// If the file empty or ended then an empty string will be returned.
		std::string getNextLine();

		// Return true if the end of the file has reached, false otherwise.
		bool isEnd();

		// Functions to report error from setFile() or any function that may report an error.
		bool isError();
		std::string getError();
};


#endif