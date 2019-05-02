#include <iostream>
#include "stack.h"
#include "calculator.h"
#include "expression.h"
using namespace std;

int main()
{
	Calculator A;

	while (1)
	{
		A.initCalculator();
		cout << endl << "������ �Է��ϼ��� (����� quit) : ";
		A.setExpresion();
		if (A.isQuit() == true)
			return 0;
		else
		{
			try
			{
				A.getPostfix();
			}
			catch (const char *exception)
			{
				cout << "[����]" << exception << endl;
			}
		}
	}
}

