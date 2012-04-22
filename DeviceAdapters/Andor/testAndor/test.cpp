/*
 * test.cpp
 *
 *  Created on: Apr 22, 2012
 *      Author: Zephyre
 */

#include <iostream>
#include "atmcd32d.h"

using namespace std;

int main() {
//	cout<<"Init: "<<Initialize("C:\\")<<endl;
	cout << "Num: ";
	at_32 num = 0;
	int a = GetAvailableCameras(&num);
	cout << a << '\t' << num << endl;
	cout << "Done." << endl;
	return 0;
}

