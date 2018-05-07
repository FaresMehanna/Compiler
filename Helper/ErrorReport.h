#ifndef ERROR_REPORT_H
#define ERROR_REPORT_H

#include <string>

class ErrorReport{
	private:
		std::string errorMsg;
		bool error;
	public:
		ErrorReport();
		void setError(std::string sError);
		bool isError();
		std::string getError();
};

#endif