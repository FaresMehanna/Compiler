#include <stdio.h>
#include <string>
#include "File.h"
#include "ErrorReport.h"
#include "FileReader.h"

using namespace std;

FileReader::FileReader() : File() {

}

bool FileReader::setFile(string fileName){

	//try to open the file
	fp = fopen(fileName.c_str(),"r");
	
	//report error in failure
	if(fp == NULL){
		error.setError("Can't open the file for reading.");
		return false;
	}
	
	//in succ return true
	return true;
}
	
string FileReader::getNextLine(){

	if(fileEnd){
		return "";
	}

	if(fp == NULL){
		error.setError("No file is opened to be read.");
		return "";
	}

	string line;
	char c;
    
    // iteratively get characters from standard input, checking for CR (Mac OS), LF (Linux), and CRLF (Windows)
    while ((c = fgetc(fp)) != '\r' && c != '\n' && c != EOF){
    	line += c;
    }

    if(c == EOF){
    	fileEnd = true;
    }

    return line;
}

bool FileReader::isEnd(){
	return fileEnd;
}

bool FileReader::isError(){
	return error.isError();
}

string FileReader::getError(){
	return error.getError();
}