#ifndef TEST_H
#define TEST_H

#include <string>
#include "Test.h"

using namespace std;


void Test::deliverError(string error){
	cout << "ERROR FROM \"" << testName << "\" : " << error << endl;

}

void Test::deliverNotification(string notification){
	cout << "NOTIFICATION FROM \"" << testName << "\" : " << notification << endl;
}

Test::Test(string testName){
	this->testName = testName;
}