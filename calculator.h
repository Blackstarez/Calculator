
#include "stack.h"
#include "expression.h"




class Calculator
{
private:
	Expression e;
public:

	bool isQuit();
	void initCalculator();
	void setExpresion();
	void getPostfix();
	int calculating(Expression &postFix);
	void getEx(int i);
	int icp(char op);		//���� ������ �켱���� ������, ���� ������ -1 ��ȯ
	int isp(char op);
};
