#include "expression.h"


Expression::Expression()
{
	size = 128;
	fill = 0;
	expresion = new char[size];
}


Expression::Expression(int newSize)
{
	size = newSize;
	fill = 0;
	expresion = new char[size];
}


Expression::~Expression()
{
	delete[] expresion;
}


void Expression::resize(int newSize)
{
	char* tmp = new char[newSize];
	for (int i = 0; i < fill; i++)
		tmp[i] = expresion[i];

	delete[] expresion;
	expresion = new char[newSize];

	for (int i = 0; i < fill; i++)
		expresion[i] = tmp[i];

	delete[] tmp;
}