#pragma once
#include <iostream>
using namespace std;

template <typename T>
class Stack
{
	T* item;
	int size;
	int top;
public:
	Stack(int stackSize = 8);
	bool isEmpty();		//비면 true  아니면 false
	void resize(int size);
	void push(T newItem);
	T pop();
	T peek();
	int getTop();
	

};

template <typename T>
Stack<T>::Stack(int stackSize)
{
	size = stackSize;
	top = -1;
	item = new T[size];
}

template <typename T>
bool Stack<T>::isEmpty()			
{
	return (top == -1);
}

template <typename T>
void Stack<T>::resize(int size)
{
	T *tmp = new T[size];
	for (int i = 0; i < size; i++)
	{
		tmp[i] = item[i];
	}
	item = tmp;
}

template <typename T>
void Stack<T>::push(T newItem)
{
	if (top + 1 == size)
		resize(size * 2);

	top++;
	item[top] = newItem;
}

template <typename T>
T Stack<T>::pop()
{
	if (isEmpty())
		throw "비어있는 스택입니다.";

	T popItem = item[top];
	item[top--] = NULL;

	if (top - 1 < size / 4)
		resize(size / 2);

	return popItem;

}

template <typename T>
T Stack<T>::peek()
{
	if (isEmpty())
		throw "비어있는 스택입니다.";
	return item[top];
}

template <typename T>
int Stack<T>::getTop()
{
	return top;
}