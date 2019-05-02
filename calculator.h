
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
	int icp(char op);		//숫자 높은게 우선순위 높은거, 숫자 들어오면 -1 반환
	int isp(char op);
};
