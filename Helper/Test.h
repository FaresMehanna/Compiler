#ifndef TEST_H
#define TEST_H

#include <string>

/*
 * Class Name : Test
 *
 * This class represent interface to the test classes to implement it.
 */
class Test{

	protected:

		// This is protected function to deliver the errors to the user, 
		// This function will be implemented in the base class incase the need
		//to change the way the errors are reported to the user.
		void deliverError(std::string error);

		// This is protected function to deliver the notifications to the user, 
		// This function will be implemented in the base class incase the need
		//to change the way the notifications are reported to the user.
		void deliverNotification(std::string notification);

		// The name of the test to be displayed in the error msg to the user.
		std::string testName;

	public:

		// Constructor to set the name		
		Test(std::string testName);
		
		// Pure virtual function wich will perform the test and deliver the notifications
		//to the user.
		virtual void performTest() = 0;

};

#endif