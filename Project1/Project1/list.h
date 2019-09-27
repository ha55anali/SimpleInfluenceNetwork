#pragma once
#include <iostream>
#include <exception>

template<class T>
class node{
public:
	node(){
		next=nullptr;
	}

	node(T d,node<T>* n=0){
		data=d;
		next=n;
	}
	T data;
	node<T>* next;
};

//singly linked list with dummy head and tail pointers
template <class T>
class sList{
public:

	class iterator {
		friend class sList;
	public:
		iterator(node<T>* p = nullptr) {
			current = p;
		}

		T& operator*() {
			return current->data;
		}

		T& operator->() {
			return current->data;
		}

		bool operator==(iterator const& it) const {
			if (current == it.current) return 1;
			return 0;
		}

		bool operator!=(iterator const& it) const {
			if (current == it.current) return 0;
			return 1;
		}

		iterator& operator++() {
			current = current->next;
			return *this;
		}

		iterator& operator++(int n) {
			node<T>* temp = current;
			current = current->next;
			return *temp;
		}

	private:
		node<T>* current;
	};

	iterator begin() {
		iterator it(head->next);
		return it;
	}

	iterator end() {
		iterator it(tail);
		return it;
	}

	sList();
	sList(sList const&);
	~sList();

	//insert operations with values
	void insertAtStart(T const& val);
	void insertAtEnd(T const& val);//uses operator=
	void insertAfter(T const& val,T const& after); //throws invalid arguemnt
	void insertAfter(T const& val, iterator it);

	//remove operations
	void remove(T val);
	void removeWithoutDel(T val);
	void remove(iterator it);

	node<T>* find(T const& val);

	template <class Temp>
	friend std::ostream& operator<<(std::ostream& os, sList<Temp> const& l);

	void print() const;
	int getSize() const { return size; }
	T& operator[](int n); //throws invalid argument
	void operator=(sList const&);
private:
	node<T>* head;
	node<T>* tail;
	int size;

	void remove(node<T>* n);
	void removeWithoutDel(node<T>* n);
};

//creats dummy head and tail
template <class T>
sList<T>::sList(){
	head=new node<T>();
	tail=new node<T>();
	head->next=tail;
	size = 0;
}

//copy constructor using insertAtEnd
template<class T>
sList<T>::sList(sList const& source):size(0)
{
	head = new node<T>();
	tail = new node<T>();
	head->next = tail;
	node<T>* sourceTemp = source.head->next;
	while (sourceTemp != source.tail) {
		insertAtEnd(sourceTemp->data);
		sourceTemp = sourceTemp->next;
	}
}

template<class T>
sList<T>::~sList()
{
	node<T>* temp = head->next;
	while (temp != tail) {
		remove(temp);
	}
	delete head;
	delete tail;
}

template <class T>
void sList<T>::insertAtStart(T const&val){
	node<T> *temp=new node<T>(val,head->next);
	head->next=temp;
	++size;
}

//copies val into tail and creates a new tail
template <class T>
void sList<T>::insertAtEnd(T const& val){
	tail->data=val;
	tail->next=new node<T>();
	tail=tail->next;
	++size;
}

template <class T>
void sList<T>::insertAfter(T const& val,T const& after){
	node<T>* prevNode=find(after);
	if (prevNode != tail) {
		node<T>* temp = new node<T>(val, prevNode->next);
		prevNode->next = temp;
		++size;
	}
	else throw std::invalid_argument("The argument passed does not exist");
}

template <class T>
node<T>* sList<T>::find(T const& val) {
	node<T>* temp=head->next;
	while (temp!=tail){
		if (temp->data==val){
			break;
		}
		temp=temp->next;	
	}
	return temp;
}

template <class T>
void sList<T>::remove(T val){
	node<T>* temp=find(val);
	if (temp != tail) {
		remove(temp);
	}
	else throw std::invalid_argument("The argument passed does not exist");
}

template<class T>
void sList<T>::removeWithoutDel(T val)
{
	node<T>* temp = find(val);
	if (temp != tail) {
		removeWithoutDel(temp);
	}
	else throw std::invalid_argument("The argument passed does not exist");

}

template <class T>
void sList<T>::print() const{
	if (head->next==tail){
		std::cout<<"empty list";		
	}
	else{
		node<T>* temp=head->next;
		while (temp!=tail){
			std::cout<<temp->data<<" ";
			temp=temp->next;
		}
	}
}

//returns data at nth position by reference
template<class T>
T& sList<T>::operator[](int n)
{
	if (n >= size|| n<0) throw std::invalid_argument("invalid index");
	else {
		int count = 0;
		node<T>* temp = head->next;
		while (temp != tail && count != n) {
			temp = temp->next;
			++count;
		}
		return temp->data;
	}

}

template<class T>
void sList<T>::operator=(sList const& s)
{
	//empties list
	node<T>* temp = head->next;
	while (temp != tail) remove(temp);

	//adds new elements
	node<T>* sTemp = s.head->next;
	while (sTemp != s.tail) {
		insertAtEnd(sTemp->data);
	}
}

template <class T>
void sList<T>::remove(node<T>* n){
	//copy next node to current
	n->data=n->next->data;
	//stores node to be deleted
	node<T>* temp=n->next;
	n->next=temp->next;
	//handles boundry case where last node is removed, updates tail
	if (temp == tail) tail = n;
	delete temp;
	--size;
}

template<class T>
void sList<T>::removeWithoutDel(node<T>* n)
{
	//copy next node to current
	n->data = n->next->data;
	//stores node to be deleted
	node<T>* temp = n->next;
	n->next = temp->next;
	//handles boundry case where last node is removed, updates tail
	if (temp == tail) tail = n;
	--size;
}

template<class Temp>
std::ostream& operator<<(std::ostream& os, sList<Temp> const& l)
{
	l.print();
	return os;
}

template<class T>
void sList<T>::remove(iterator it){
	remove(it.current);
}

template<class T>
void sList<T>::insertAfter(T const& val, iterator it){
	node<T>* temp=new node<T>(val,it.current->next);
	it.current->next=temp;
}
