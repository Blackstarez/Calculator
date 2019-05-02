#include <iostream>
#include "stack.h"
#include "calculator.h"
using namespace std;


bool Calculator::isQuit()
{
	return (toupper(e.expresion[0]) == 'Q' && toupper(e.expresion[1]) == 'U' && toupper(e.expresion[2]) == 'I' && toupper(e.expresion[3]) == 'T');
	//return strcmp(e.expresion, "quit");
}

void Calculator::initCalculator()
{
	for (int i = 0; i < e.fill; i++)
		e.expresion[i] = NULL;
	e.fill = 0;
}


void Calculator::setExpresion()
{
	int i;
	while (1)
	{
		if ((char)(i = cin.get()) == '\n')
			break;
		if ((e.fill + 1) == e.size)
			e.resize(e.size * 2);
		if ((char)i == ' ')
			continue;
		e.expresion[e.fill] = (char)i;
		e.fill++;
	}
	if (e.fill < e.size / 4)
		e.resize(e.size / 2);
}
void Calculator::getPostfix()
{
	enum {number, spo, dpo,close};		// 숫자 spo==single-port operator(단항연산자) dpo==double-port operator이항연산자  close==닫는괄호
	Stack<char> operators;
	Expression postFix;
	int previous=dpo;
	int count = 0;

	for (int i = 0; i < e.fill; i++)
	{
		if (e.expresion[e.fill-1] < '0' && '9' < e.expresion[e.fill-1] && e.expresion[e.fill-1]!=')')
			throw "식의 끝이 숫자 또는 닫는괄호가 아닙니다.";
		if ('0'<= e.expresion[i] && e.expresion[i] <= '9')		//지금이 number인경우
		{
			if (previous == close)
			{
				throw "닫는괄호 뒤에 숫자가 입력되었습니다.";
			}
			postFix.expresion[postFix.fill] = e.expresion[i];
			postFix.fill++;
			previous = number;
			continue;
		}
		if (2 <= icp(e.expresion[i]) && icp(e.expresion[i]) <= 5)		//지금이 연산자인경우
		{
			
			
			if (previous == dpo||previous==spo)
			{
				if (e.expresion[i] == '-')
				{
					operators.push('~');
					previous = spo;
					continue;
				}
				else
					throw "이항연산자 앞에 숫자가 존재하지 않습니다.";
			}
			else
			{
				postFix.expresion[postFix.fill++] = ' ';
					while (!operators.isEmpty() &&icp(e.expresion[i]) <= isp(operators.peek()))
					{
						postFix.expresion[postFix.fill++] = operators.pop();
						postFix.expresion[postFix.fill++] = ' ';
					}
				

				operators.push(e.expresion[i]);
				previous = dpo;
				continue;
			}
		}
		
		if (isp(e.expresion[i]) == 1)
		{
			if (e.expresion[i] == '(')
			{
				if (previous == number || previous == close)
					throw "여는괄호앞에 숫자 또는 닫는괄호가 존재합니다.";

				operators.push('(');
				count++;
				continue;
			}
			if (e.expresion[i] == ')')
			{
				if (previous != number)
					throw "닫는괄호 앞이 숫자가 아닙니다.";
				if (count == 0)
					throw "여는괄호와 닫는괄호의 수가 일치하지 않습니다.";
				while (operators.peek() != '(')
				{
					postFix.expresion[postFix.fill++] = ' ';
					postFix.expresion[postFix.fill++] = operators.pop();
				}
				operators.pop();
				count--;
			}
			

		}
		
	}

	while(!operators.isEmpty())
	{
		postFix.expresion[postFix.fill++] = ' ';
		postFix.expresion[postFix.fill++] = operators.pop();
			
	}

	if (count != 0)
		throw "여는괄호와 닫는괄호의 수가 일치하지 않습니다.";

	cout << "후위 연산식은 : ";
	for (int i = 0; i < postFix.fill; i++)
	{
		cout << postFix.expresion[i];
	}
	cout << endl;


	try
	{
		cout << "결과값은 : " << calculating(postFix);
	}
	catch (const char *exception)
	{
		cout << "[오류]" << exception << endl;
	}

}


int Calculator::calculating(Expression &postFix)
{
	Stack<int> number;
	char charTonum[10];
	int charPoint = 0;		//charTonum에 들어간 값의 갯수
	int prePoint = NULL;		//현재 가르키는 지점 이전 지점을 가르킨다.
	int tmp;				//dpo 연산시 사용되는 임시 변수
	int tmp2;				//제곱 연산시 사용되는 임시 변수

	for (int i = 0; i < postFix.fill; i++)
	{
		if (postFix.expresion[i] == ' ')
		{
			if (atoi(charTonum) != 0)
				number.push(atof(charTonum));
			for (int j = 0; j < charPoint; j++)
				charTonum[j] = NULL;
			charPoint = 0;
			continue;
		}
		else if (postFix.expresion[i] == '0'&&icp(postFix.expresion[i]) < 0)			//70 60 이런 number 제외하고 0 인 경우만 넣어야함.
		{
			if (charPoint == 0)						//0인경우
			{
				number.push(0);
			}
			else
			{
				charTonum[charPoint] = postFix.expresion[i];
				charPoint++;
			}
		}
		else if (icp(postFix.expresion[i]) < 0)		//number인 경우
		{
			charTonum[charPoint] = postFix.expresion[i];
			charPoint++;
		}
		else										//문자인 경우
		{

			if (postFix.expresion[i] == '~')		//spo연산자
			{
				number.push(number.pop()*-1);
			}
			else								//dpo연산자
			{
				if (number.getTop() < 1)		//dpo연산자를 수행할 number의 수가 부족 ex) '+'연산을 수행해야하는데 인자가 1개 또는 0개인 경우
					throw "이해할 수 없는 수식";
				switch (postFix.expresion[i])
				{
				case '+':
					number.push(number.pop() + number.pop());
					break;
				case '-':
					tmp = number.pop();
					number.push(number.pop() - tmp);
					break;
				case '*':
					number.push(number.pop()*number.pop());
					break;
				case '^':
					tmp = number.pop();
					tmp2 = number.peek();
					if (tmp > 0)
					{
						for (int i = 1; i < tmp; i++)
							number.push(number.pop()*tmp2);
						break;
					}
					else if (tmp == 0)
					{
						number.push(number.pop() / tmp2);
					}
					else				//음수 승수는 무조건 분수가 나온다 -> int형으로 저장하기 때문에 무조건 0이 저장됨.
					{
						number.pop();
						number.push(0);
					}
					break;
				case '/':
					if (number.peek() == 0)		//나누는 수가 0일경우 ->오류
					{
						throw "나누기의 두번째 연산자가 0인 경우가 발생";
					}
					else
					{
						tmp = number.pop();
						number.push(number.pop() / tmp);
					}
					break;
				case '%':
					if (number.peek() == 0)		//나누는 수가 0일경우 ->오류
					{
						throw "나머지연산의 두번째 연산자가 0인 경우가 발생";
					}
					else
					{
						tmp = number.pop();
						number.push((int)number.pop() % (int)tmp);
					}
					break;
				}
			}



		}
	}
	return number.peek();
}

void Calculator::getEx(int i)
{
	cout << e.expresion[i] << endl;
}


int Calculator::icp(char op)		//number 높은게 우선순위 높은거 ㅇㅋ? ㅇㅇㅋ  number 들어오면 -1 반환
{
	switch (op)
	{
	case '+':
	case '-':
		return 2;
		break;
	case '*':
	case '/':
	case '%':
		return 3;
		break;
	case '^':
		return 4;
		break;
	case '~':
		return 5;
		break;
	case '(':
		return 6;
		break;
	case ')':
		return 1;
		break;
	default:
		return -1;
		break;
	}
}


int Calculator::isp(char op)
{
	switch (op)
	{
	case '+':
	case '-':
		return 2;
		break;
	case '*':
	case '/':
	case '%':
		return 3;
		break;
	case '^':
		return 4;
		break;
	case '~':
		return 5;
		break;
	case '(':
	case ')':
		return 1;
		break;

	}
}
