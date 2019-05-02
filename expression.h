#pragma once


class Expression
{
	friend class Calculator;
	char *expresion;
	int size;
	int fill;
public:
	Expression();
	Expression(int newSize);
	~Expression();
	void resize(int newSize);

};