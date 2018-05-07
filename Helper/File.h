#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include "ErrorReport.h"

class File{

	protected:
		FILE* fp;
		ErrorReport error;
		bool fileEnd;
		
		File(){
			fileEnd = false;
			fp = NULL;
		}

		~File(){
			if(fp){
				fclose(fp);
			}
		}
};


#endif