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

bool ErrorReport::isError(){
	return error;
}

string ErrorReport::getError(){
	return errorMsg;
}
