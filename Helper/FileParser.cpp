#include <string>
#include "ErrorReport.h"
#include "FileParser.h"

using namespace std;

bool FileParser::setFile(const std::string& fileName){
	
	//try to open the file
	bool succ = reader.setFile(fileName));

	//if failed, set the error message and return false
	if(!succ){
		error.setError(reader.getError());
		return false;
	}

	return true;
}

bool FileParser::isError(){
	return error.isError();
}

std::string FileParser::getError(){
	return error.getError();
}