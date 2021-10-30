#ifndef  _MYSTACK_H_ 
#define MYSTACK_H
#include<iostream>
using namespace std;

template <class dataType>//template class
class mystack
 {
 public:
	 mystack()
	 {
		 top1 = NULL;//Initialize the pointer field to null
		 length = 0;//Initialize the length with 0
	 }

	 ~mystack()//Destructor
	 {
		 node* ptr = NULL;
		 while (top1 != NULL) {
			 ptr = top1->next;
			 delete top1;
			 top1 = ptr;
		 }
	 }

	void push(dataType a)//push an element into the stack 
	{
		node* ptr = new node;
		ptr->val = a;          
		ptr->next = top1;       
		top1 = ptr;        
		length = length+1;
	}

	void pop()//pop out the stack top
	{
		node* ptr = top1->next;      
		delete top1;               
		top1 = ptr;               
		length = length-1;
	}

	dataType top()//return the top but not pop
	{
		return top1->val;  
	} 

	bool empty()
	{
		return top1 == NULL;         
	}

	int size()
	{
		return length;
	}

 private:
	 struct node//define the lined list node   
	 {
		 dataType val;   
		 node* next;     
	 };
	 node * top1;//pointer field                  
	 int length;//
};

#endif