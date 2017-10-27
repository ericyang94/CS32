// Homework 3 Problem 5

#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;

bool precedence(char operand1, char operand2);
bool isNum(char ch);

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	postfix = "";
	stack<char> charStack;
	char ch, operand1, operand2;
	char lastch = ' ';
	int counter = 0;

	if (infix.compare("") == 0)
		return 1;
	if (infix[0] == '&' || infix[0] == '|')
		return 1;
	if (infix[infix.size()-1] == '&' || infix[infix.size()-1] == '|' || infix[infix.size()-1] == '!')
		return 1;

	for(int k = 0; k != infix.size(); k++)
	{
		ch = infix[k];

		if (ch == ' ')
			continue;
		
		if (!isNum(ch) && ch != '!' && ch != '|' && ch != '&' && ch != '(' && ch != ')')
			return 1;

		if (k!=0)
		{
			if (ch != '!' && ch != '(' && ch != ')' && ch == lastch)
				return 1;
			if (ch == '!' && (isNum(lastch) || lastch == ')'))
				return 1;
			if (ch == '(' && (lastch == ')' || isNum(lastch)))
				return 1;
			if (ch == ')' && (lastch == '!' || lastch == '&' || lastch == '|' || lastch == '('))
				return 1;
			if (ch == '&' && (lastch == '|' || lastch == '!' || lastch == ')' || lastch == '('))
				return 1;
			if (ch == '|' && (lastch == '&' || lastch == '!' || lastch == ')' || lastch == '('))
				return 1;
			if (isNum(ch) && (lastch == ')' || isNum(lastch)))
				return 1;
		}

		switch (ch)
		{
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
				postfix += ch;
				break;
			case '!':
				charStack.push(ch);
				break;
			case '(':
				charStack.push(ch);
				counter++;
				break;
			case ')':
				while(charStack.top() != '(')
				{
					postfix += charStack.top();
					charStack.pop();
				}
				charStack.pop();
				counter--;
				break;
			case '|':
			case '&':
				while(!charStack.empty() && charStack.top() != '(' && precedence(ch,charStack.top()))
				{
					postfix += charStack.top();
					charStack.pop();
				}
				charStack.push(ch);
				break;
		}
		lastch = infix[k];
	}

	if (counter != 0)
		return 1;

	while(!charStack.empty())
	{
		postfix += charStack.top();
		charStack.pop();
	}

	stack<char> charStack2;

	for(int k = 0; k != postfix.size(); k++)
	{
		ch = postfix[k];
		if(ch != '|' && ch != '&' && ch != '!')
			charStack2.push(ch);
		else if (ch == '!')
		{
			operand1 = charStack2.top();
			charStack2.pop();
			if (values[operand1 - '0'])
			{
				for(int i = 0; i !=10; i++)
					if (!values[i])
					{
						charStack2.push(i+'0');
						break;
					}
			}
			else
				for(int i = 0; i !=10; i++)
					if (values[i])
					{
						charStack2.push(i+'0');
						break;
					}
		}
		else
		{
			operand1 = charStack2.top();
			charStack2.pop();
			operand2 = charStack2.top();
			charStack2.pop();
			if (ch == '|')
			{
				if (values[operand1 - '0'])
					charStack2.push(operand1);
				else charStack2.push(operand2);
			}
			if (ch == '&')
			{
				if (values[operand1 - '0'] && !values[operand2 - '0'])
					charStack2.push(operand2);
				else charStack2.push(operand1);
			}
		}
	}

	result = values[charStack2.top() - '0'];
	return 0;
}

bool precedence(char operand1, char operand2)
{
	if (operand1 == operand2 || operand1 == '|' || operand1 == '&' && operand2 == '!')
		return true;
	else
		return false;
}

bool isNum(char ch)
{
	switch (ch)
	{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
			return true;
		default:
			return false;
	}
}

int main()
{
    bool ba[10] = {
      //  0      1      2      3      4      5      6      7      8      9
        true,  true,  true,  false, false, false, true,  false, true,  false
    };
    string pf;
    bool answer;
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  answer);
    assert(evaluate("8|", ba, pf, answer) == 1);
    assert(evaluate("4 5", ba, pf, answer) == 1);
    assert(evaluate("01", ba, pf, answer) == 1);
    assert(evaluate("()", ba, pf, answer) == 1);
    assert(evaluate("2(9|8)", ba, pf, answer) == 1);
    assert(evaluate("2(&8)", ba, pf, answer) == 1);
    assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
    assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
                           &&  pf == "43!03&&|"  &&  !answer);
    assert(evaluate("", ba, pf, answer) == 1);
    assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
    ba[2] = false;
    ba[9] = true;
    assert(evaluate("!!!!9&!!9&!2|2", ba, pf, answer) == 0  &&  pf == "9!!!!9!!&2!&2|"  &&  answer);
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);
    cout << "Passed all tests" << endl;
}