#ifndef LINE_H
#define LINE_H

#include <stdio.h>
#include <string>

/*
 * Class Name : Line
 *
 * This class represent single line from any file.
 */
class RuleLine : public Line{

	public:

		// Get the number of the line inside the file.
		virtual int getLineNumber() = 0;

		// Get the string of the line inside the file.
		virtual std::string getLineText() = 0;

};

#endif