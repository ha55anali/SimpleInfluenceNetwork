#pragma once
#include "list.h"
#include "Stack.h"

#include <string>
#include <fstream>
#include <exception>

class InfluenceNetwork {
public:
	InfluenceNetwork();
	~InfluenceNetwork();

	void Input(std::string path);
	void Calculate_Influence();

	void print();
private:
	sList<int>* network;
	int UserSize;

	void checkNetworkConsistency();
	void checkFile(std::string path);
	void readUsers(std::ifstream& file);
	void addUserToNetwork(std::string lot, int User);

	void RecAdd(int const User, Stack<int>& s, bool* arr, int size);
	Stack<int>* CalInfluence();

	int GetMaxInfluence(Stack<int>*);
	int strToInt(std::string);
};

InfluenceNetwork::InfluenceNetwork(){
	network=nullptr;
	UserSize = 0;
}


InfluenceNetwork::~InfluenceNetwork(){
	delete[] network;
}


void InfluenceNetwork::Input(std::string path){
	checkFile(path);

	using namespace std;
	ifstream file;
	file.open(path);

	//get number of users
	int UserCount = 0;
	std::string tempStr;
	getline(file, tempStr);
	try {
		UserCount = strToInt(tempStr);
	}
	catch (invalid_argument & e) {
		file.close();
		throw e;
	}

	network = new sList<int>[UserCount+1];
	UserSize = UserCount;

	try{
		readUsers(file);
	}
	catch (invalid_argument& e) {
		file.close();
		throw e;
	}
	file.close();

	checkNetworkConsistency();
}

void InfluenceNetwork::Calculate_Influence()
{
	Stack<int>* stack = CalInfluence();

	for (int c = 1; c < UserSize+1; ++c) {
		std::cout << c << "| influences: " << stack[c].getSize() << ": "; 
		stack[c].print();
		std::cout << std::endl;
	}

	std::cout<<"User with maximum influence: "<<GetMaxInfluence(stack);
	delete[] stack;
}

Stack<int>* InfluenceNetwork::CalInfluence()
{
	Stack<int>* stackArr = new Stack<int>[UserSize+1];

	for (int c = 1; c < UserSize+1; ++c) {
		bool* arr = new bool[UserSize+1];
		for (int x = 0; x < UserSize+1; ++x) arr[x] = 0;
		arr[c] = 1;

		RecAdd(c, stackArr[c], arr, UserSize);
	}

	return stackArr;
}

void InfluenceNetwork::print()
{
	if (UserSize == 0)
		std::cout << "empty network";

	for (int c = 1; c < UserSize+1; ++c) {
		network[c].print();
		std::cout << std::endl;
	}
}

//checks if network is consistent
void InfluenceNetwork::checkNetworkConsistency(){
	for (int c=1;c<UserSize+1;++c){
		if (c!=*network[c].begin())
			throw std::exception("Inconsistent User IDs");
	}

	for (int c=1;c<UserSize+1;++c){
		for (sList<int>::iterator it=network[c].begin();it!=network[c].end();++it){
			if (*it>UserSize || *it<1)
				throw std::exception("inconsistent friends");
		}
	}
}

//checks if file can be opened and number of users is consistent
void InfluenceNetwork::checkFile(std::string path){
	using namespace std;
	ifstream file;
	file.open(path);

	if (file.fail()) {
		file.close();
		throw invalid_argument("File cannot be opened");
	}

	int UserCount = 0;
	std::string tempStr;
	getline(file, tempStr);
	UserCount = strToInt(tempStr);

	int LineCount=0;
	while (!file.eof()) {
		getline(file, tempStr);
		++LineCount;
	}

	if (UserCount != LineCount) {
		file.close();
		throw invalid_argument("File is corrupt");
	}

	file.close();
}

 void InfluenceNetwork::readUsers(std::ifstream& file)
{
	 std::string tempStr;
	 int count = 1;

	 //read till end of file
	 while (count<UserSize+1) {

		 //line of text
		 std::string lot;
		 getline(file, lot);

		 addUserToNetwork(lot, count);
		 
		 ++count;
	 }
}

 void InfluenceNetwork::addUserToNetwork(std::string lot, int User)
 {
	 std::string tempStr;
	 std::string delimiter = "-";
	 size_t pos = 0;

	 //get user id
	 pos = lot.find(delimiter);
	 int temp;
	 temp =strToInt(lot.substr(0, pos));
	 lot.erase(0, pos + 1);
	 network[User].insertAtEnd(temp);

	 //get user friends
	 delimiter = ",";
	 while ((pos = lot.find(delimiter)) != std::string::npos) {
		 tempStr = lot.substr(0, pos);
		 temp =strToInt(tempStr);
		 lot.erase(0, pos + delimiter.length());
		 network[User].insertAtEnd(temp);
	 }
	 temp = strToInt(lot);
	 network[User].insertAtEnd(temp);
 }

 void InfluenceNetwork::RecAdd(int const User, Stack<int>& s, bool* arr, int size)
 {
	 //base case no friends
	 if (network[User].getSize() == 1) {
		 return;
	 }


	 for (sList<int>::iterator it = ++network[User].begin(); it != network[User].end(); ++it) {
		 if (arr[*it] == 0) {
			 s.push(*it);
			 arr[*it] = 1;
			 RecAdd(*it, s, arr, size);
		 }
	 }
 }

int InfluenceNetwork::GetMaxInfluence(Stack<int>* s){
	int maxVal=0;
	int maxUser=0;

	for (int c=1;c<UserSize+1;++c){
		if (s[c].getSize()>maxVal){
			maxVal=s[c].getSize();
			maxUser=c;
		}
	}

	return maxUser;
}

 int InfluenceNetwork::strToInt(std::string str)
{
	 try {
		 return stoi(str);
	 }
	 catch (std::invalid_argument & e) {
		 throw std::invalid_argument("Input file is corrupt. STOI exception");
	 }
}
