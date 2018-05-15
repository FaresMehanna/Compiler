#include <iostream>
#include <string>
#include "ErrorReport.h"

using namespace std;


ErrorReport::ErrorReport(){
	errorMsg = "";
	error = false;
}

void ErrorReport::setError(string sError){
	error = true;
	errorMsg = sError;
}

void ErrorReport::setError(string sError, int lineOfError = -1){
	error = true;
	if(lineOfError == -1){
		errorMsg = sError;
	}else{
		errorMsg = "At Line " + to_string(lineOfError) + ". " + sError;
	}
}

bool ErrorReport::isError(){
	return error;
}

string ErrorReport::getError(){
	return errorMsg;
}
