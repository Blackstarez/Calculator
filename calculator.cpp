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
	enum {number, spo, dpo,close};		// ���� spo==single-port operator(���׿�����) dpo==double-port operator���׿�����  close==�ݴ°�ȣ
	Stack<char> operators;
	Expression postFix;
	int previous=dpo;
	int count = 0;

	for (int i = 0; i < e.fill; i++)
	{
		if (e.expresion[e.fill-1] < '0' && '9' < e.expresion[e.fill-1] && e.expresion[e.fill-1]!=')')
			throw "���� ���� ���� �Ǵ� �ݴ°�ȣ�� �ƴմϴ�.";
		if ('0'<= e.expresion[i] && e.expresion[i] <= '9')		//������ number�ΰ��
		{
			if (previous == close)
			{
				throw "�ݴ°�ȣ �ڿ� ���ڰ� �ԷµǾ����ϴ�.";
			}
			postFix.expresion[postFix.fill] = e.expresion[i];
			postFix.fill++;
			previous = number;
			continue;
		}
		if (2 <= icp(e.expresion[i]) && icp(e.expresion[i]) <= 5)		//������ �������ΰ��
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
					throw "���׿����� �տ� ���ڰ� �������� �ʽ��ϴ�.";
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
					throw "���°�ȣ�տ� ���� �Ǵ� �ݴ°�ȣ�� �����մϴ�.";

				operators.push('(');
				count++;
				continue;
			}
			if (e.expresion[i] == ')')
			{
				if (previous != number)
					throw "�ݴ°�ȣ ���� ���ڰ� �ƴմϴ�.";
				if (count == 0)
					throw "���°�ȣ�� �ݴ°�ȣ�� ���� ��ġ���� �ʽ��ϴ�.";
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
		throw "���°�ȣ�� �ݴ°�ȣ�� ���� ��ġ���� �ʽ��ϴ�.";

	cout << "���� ������� : ";
	for (int i = 0; i < postFix.fill; i++)
	{
		cout << postFix.expresion[i];
	}
	cout << endl;


	try
	{
		cout << "������� : " << calculating(postFix);
	}
	catch (const char *exception)
	{
		cout << "[����]" << exception << endl;
	}

}


int Calculator::calculating(Expression &postFix)
{
	Stack<int> number;
	char charTonum[10];
	int charPoint = 0;		//charTonum�� �� ���� ����
	int prePoint = NULL;		//���� ����Ű�� ���� ���� ������ ����Ų��.
	int tmp;				//dpo ����� ���Ǵ� �ӽ� ����
	int tmp2;				//���� ����� ���Ǵ� �ӽ� ����

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
		else if (postFix.expresion[i] == '0'&&icp(postFix.expresion[i]) < 0)			//70 60 �̷� number �����ϰ� 0 �� ��츸 �־����.
		{
			if (charPoint == 0)						//0�ΰ��
			{
				number.push(0);
			}
			else
			{
				charTonum[charPoint] = postFix.expresion[i];
				charPoint++;
			}
		}
		else if (icp(postFix.expresion[i]) < 0)		//number�� ���
		{
			charTonum[charPoint] = postFix.expresion[i];
			charPoint++;
		}
		else										//������ ���
		{

			if (postFix.expresion[i] == '~')		//spo������
			{
				number.push(number.pop()*-1);
			}
			else								//dpo������
			{
				if (number.getTop() < 1)		//dpo�����ڸ� ������ number�� ���� ���� ex) '+'������ �����ؾ��ϴµ� ���ڰ� 1�� �Ǵ� 0���� ���
					throw "������ �� ���� ����";
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
					else				//���� �¼��� ������ �м��� ���´� -> int������ �����ϱ� ������ ������ 0�� �����.
					{
						number.pop();
						number.push(0);
					}
					break;
				case '/':
					if (number.peek() == 0)		//������ ���� 0�ϰ�� ->����
					{
						throw "�������� �ι�° �����ڰ� 0�� ��찡 �߻�";
					}
					else
					{
						tmp = number.pop();
						number.push(number.pop() / tmp);
					}
					break;
				case '%':
					if (number.peek() == 0)		//������ ���� 0�ϰ�� ->����
					{
						throw "������������ �ι�° �����ڰ� 0�� ��찡 �߻�";
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


int Calculator::icp(char op)		//number ������ �켱���� ������ ����? ������  number ������ -1 ��ȯ
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
