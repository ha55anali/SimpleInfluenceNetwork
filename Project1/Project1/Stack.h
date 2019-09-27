#pragma once

#include "list.h"

template<class T>
class Stack{
public:
	Stack();
	~Stack();

	void pop();
	void push(T const&);

	T& TopElement();

	bool isFull();
	bool isEmpty();
	int getSize() { return s.getSize(); };

	void print();
private:
	sList<T> s;
};

template<class T>
Stack<T>::Stack(){

}

template<class T>
Stack<T>::~Stack(){

}

template<class T>
void Stack<T>::pop(){
	s.remove(s.begin());
}

template<class T>
void Stack<T>::push(T const& val){
	s.insertAtStart(val);
}

template<class T>
T& Stack<T>::TopElement()
{
	if (!isEmpty()){
		return *s.begin();
	}
}

template<class T>
bool Stack<T>::isFull()
{
	return false;
}

template<class T>
bool Stack<T>::isEmpty()
{
	if (s.getSize()==0) return 1;
	return 0;
}

template<class T>
void Stack<T>::print()
{
	s.print();
}
