// @Author Christian Garcia
// Using a stack to evaluate postfix arithmetic expressions.

#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <stack>
#include <bits/stdc++.h>
using namespace std;

// Intializing methods for Postfix class
class Postfix {
	public:
		string infixToPostfix(string expressInfix);
		int evaluatePostfix(string s);
		int evaluateExpression(int num1, int num2, char op);
		bool checkNum(char c);
		bool checkOp(char c);
		int precedence(char c);
		int to_int(string s);
		stack<int> postfix;
		
};

// Main method that asks user for an input in normal infix notation
// After input is entered, the expression will be modified to postfix
// Both the modified expression and answer will be displayed
int main() {
	Postfix p;
	string expressInfix;
	cout << "Enter an expression to be solved: " << endl;
	cin >> expressInfix;
	expressInfix = p.infixToPostfix(expressInfix);
	cout << endl << "Infix to postfix conversion: " << endl << expressInfix << endl;
	int expressPostfix = p.evaluatePostfix(expressInfix);
	cout << endl << "Postfix evaluation:" << endl << expressPostfix;
}

string Postfix::infixToPostfix(string expressInfix) {
	string expressPostfix = "";
	string currentNum = "";
	int offset = 0;
	stack<char> changeNotation;
	
	for (int i = 0; i <= expressInfix.size() - 1; i++) {
		// Concatenate currentNum string if current position in expressInfix is a numeric value
		if (expressInfix[i] >= '0' && expressInfix[i] <= '9') {
			while (checkNum(expressInfix[i]) &&  i <= expressInfix.size() + 1) {
				currentNum.insert(currentNum.end(), expressInfix[i]);
				i++;
			}
			
			expressPostfix += (currentNum + " ");
			currentNum = "";
			i--;
		}
		
		// Push onto stack if position is an opening brace
		else if (expressInfix[i] == '(') {
			changeNotation.push(expressInfix[i]);
		}
		
		// If position is a closing brace, push operands in stack into string
		else if (expressInfix[i] == ')') {
			while (changeNotation.top() != '(') {
				expressPostfix.insert(expressPostfix.end(), changeNotation.top());
				expressPostfix += " ";
				changeNotation.pop();
			}
			
			changeNotation.pop();
		}
		
		// If position is not a number or brace, position must be a operand
		else {
			// If stack is empty, push position onto stack
			if (changeNotation.empty()) {
				changeNotation.push(expressInfix[i]);
			}
			
			//Else this must mean that other operators exist and will have to check precedence of the operands
			else if (precedence(expressInfix[i]) <= precedence(changeNotation.top())) {
				expressPostfix.insert(expressPostfix.end(), changeNotation.top());
				expressPostfix += " ";
				changeNotation.pop();
				
				while (precedence(expressInfix[i]) == precedence(changeNotation.top())) {
					expressPostfix.insert(expressPostfix.end(), changeNotation.top());
					expressPostfix += " ";
					changeNotation.pop();
					
					if(changeNotation.empty()) {
						break;
					}
				}
				
				changeNotation.push(expressInfix[i]);
			}
			
			else if (precedence(expressInfix[i]) > precedence(changeNotation.top())) {
				changeNotation.push(expressInfix[i]);
			}
		}
	}
	
	// Push back whatever is leftover from the stack onto the string
	while (!changeNotation.empty()) {
		expressPostfix.insert(expressPostfix.end(), changeNotation.top());
		changeNotation.pop();
	}
	
	return expressPostfix;
}

int Postfix::evaluatePostfix(string s) {
	for (int i = 0; i < s.size(); i++) {
		// If we see an operator, pop the top two elements and evaluate the expression depending on the operator
		if (checkOp(s.at(i))) {
			int num1 = postfix.top();
			postfix.pop();
			
			int num2 = postfix.top();
			postfix.pop();
			
			int expression = evaluateExpression(num1, num2, s.at(i));
			// Push the evaluated expression back onto the stack
			postfix.push(expression);
		}
		// If we see a number, check for multiple digits in the number and then push onto stack
		else if (checkNum(s.at(i))) {
			string currentNum = "";
			
			while (checkNum(s[i]) &&  i <= s.size() + 1) {
				currentNum.insert(currentNum.end(), s[i]);
				i++;
			}

			i--;
			postfix.push(to_int(currentNum));
		}
	}
	
	// Once all calculations are solved, the remaining int in the stack is the final solution.
	return postfix.top();
}

// Calculates two numbers based on an operator
int Postfix::evaluateExpression (int num1, int num2, char op) {
	int evaluation;
	
	switch(op) {
		case '+':
			evaluation = num2 + num1;
			break;
		case '-':
			evaluation = num2 - num1;
			break;
		case '*':
			evaluation = num2 * num1;
			break;
		case '/':
			evaluation = num2 / num1;
			break;
		case '^':
			evaluation = pow(num2, num1);
			break;
	}
	
	return evaluation;
}

// Checks to see if the character inputted is a number
bool Postfix::checkNum(char c) {
	if (c >= '0' && c <= '9') {
		return true;
	}
	return false;
}

// Checks to see if the character inputted is an operator
bool Postfix::checkOp(char c) {
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')  {
		return true;
	}
	return false;
}

// Evaluate the precedence of each operator. This will be used for order of operations.
int Postfix::precedence(char c) {
	if (c == '+' || c == '-') {
		return 1;
	}
	else if (c == '*' || c == '/') {
		return 2;
	}
	else if (c == '^') {
		return 3;
	}
	
	return 0;
}

// Inputs a string and returns the string as an int
int Postfix::to_int(string s) { 
	return atoi(s.c_str()); 
}
