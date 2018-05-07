#include <string>
#include "ErrorReport.h"
#include "FileParser.h"

using namespace std;

FileParser::FileParser(const std::string& FileName){
	this->FileName = FileName;
}

bool FileParser::isError(){
	return error.isError();
}

std::string FileParser::getError(){
	return error.getError();
}