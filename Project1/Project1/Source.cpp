#include <iostream>
#include "InfluenceNetwork.h"

using namespace std;

int main() {
	InfluenceNetwork temp;
	try {
		temp.Input("test.txt");
		cout << "list view" << "--------------------------" << endl;
		temp.print();
		cout << endl << "influence calculation" << "---------------------------" << endl;
		temp.Calculate_Influence();
	}
	catch (invalid_argument & e) {
		cout << e.what();
	}
	catch (exception & e) {
		cout << e.what();
	}
}