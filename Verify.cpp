#include "Fraction.h"
#include "Expression.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
using namespace std;

bool is_number(char c)
{
	return '0' <= c && c <= '9';
}

int64_t read_integer(string str, int& index)
{
	bool is_negative = false;

	if(str[index] == '-')
	{
		is_negative = true;
		index++;
	}

	int64_t num = 0;
	if (is_number(str[index]))
	{
		do {
			num = num * 10 + (str[index] - '0');
			index++;
		} while (is_number(str[index]));
	}
	else 
	{
		fprintf(stderr, "整数格式错误");
	}

	if(is_negative)
	{
		num = -num;
	}

	return num;
}

Fraction read_number(string str, int &index)
{
	Fraction f;
	int64_t numbers[3];
	int cnt = 0;

	numbers[cnt++] = read_integer(str, index);
	if (str[index] == '\'')
	{
		index++;
		numbers[cnt++] = read_integer(str, index);
		index++;
		numbers[cnt++] = read_integer(str, index);
	} 
	else if (str[index] == '/')
	{
		index++;
		numbers[cnt++] = read_integer(str, index);
	}

	if (cnt == 1) // 整数
	{
		f.numerator = numbers[0];
		f.denominator = 1;
	}
	else if (cnt == 2) // 真分数
	{
		f.numerator = numbers[0];
		f.denominator = numbers[1];
	}
	else if (cnt == 3) // 带分数
	{
		f.numerator = numbers[0] * numbers[2] + numbers[1];
		f.denominator = numbers[2];
	}

	return f;
}

int operator_precedence(string op)
{
	if(op == "+" || op == "-")
	{
		return 1;
	}
	else if(op == "×" || op == "÷")
	{
		return 2;
	} 
	else if(op == "(")
	{
		return 0;
	}
	return -1;
}

void eval(stack<Expression *> &exprs, stack<string> &ops)
{
	Expression* expr = expression_create();
	expr->right = exprs.top();
	exprs.pop();
	expr->left = exprs.top();
	exprs.pop();
	string op = ops.top();
	ops.pop();
	if(op == "+")
	{
		expr->type = EXPRESSION_TYPE_ADD; 
	}
	else if(op == "-")
	{
		expr->type = EXPRESSION_TYPE_SUB; ;
	}
	else if(op == "×")
	{
		expr->type = EXPRESSION_TYPE_MULTIPLY;
	}
	else if(op == "÷")
	{
		expr->type = EXPRESSION_TYPE_DIVIDE;
	} 
	else
	{

		expr->type = EXPRESSION_TYPE_NUMBER; 
		fprintf(stderr, "未知的操作符\n");
	}

	exprs.push(expr);
}

Expression * read_expression(string str, int index)
{
	stack<Expression *> exprs;
	stack<string> ops;


	while(str[index] != '\n' && str[index] != '\0')
	{
		if (is_number(str[index]))
		{
			Fraction num = read_number(str, index);
			Expression* expr = expression_create();
			expr->type = EXPRESSION_TYPE_NUMBER;
			expr->number = num;
			exprs.push(expr);
		}
		else if (str[index] == '(')
		{
			string op;
			op += str[index];
			ops.push(op);
			index++;
		}
		else if (str[index] == ')')
		{
			while (ops.top() != "(")
			{
				eval(exprs, ops);
			}
			ops.pop();
			index++;
		}
		else
		{
			string op;

			if(str[index] == '\xC3' && str[index + 1] == '\xB7')
			{
				op = "÷";
				index++;
			}
			else if(str[index] == '\xC3'  && str[index + 1] == '\x97')
			{
				op = "×";
				index++;
			} 
			else
			{
				
				op += str[index];
			}

			while (ops.size() > 0 && operator_precedence(op) <= operator_precedence(ops.top()))
			{
				eval(exprs, ops);
			}
			ops.push(op);
			index++;
		}


	}

	while (!ops.empty())
	{
		eval(exprs, ops);
	}
	
	return exprs.top();
}

bool verify_oneline(string problem_str, string answer_str)
{
	// 去除所有空格
	problem_str.erase(remove(problem_str.begin(), problem_str.end(), ' '), problem_str.end());
	answer_str.erase(remove(answer_str.begin(), answer_str.end(), ' '), answer_str.end());
	int problem_str_index = 0, answer_str_index = 0;
	// 去掉等于号
	problem_str.erase(remove(problem_str.begin(),problem_str.end(),'='),problem_str.end());
	// 跳转到内容最开始部分，即.后面
	while (problem_str[problem_str_index++] != '.');
	while (answer_str[answer_str_index++] != '.');
	Expression * expr = read_expression(problem_str, problem_str_index);
	Fraction answer = read_number(answer_str, answer_str_index);

	return fraction_compare(expression_compute(expr), answer) == 0;
}

void verify(string problem_file_path, string answer_file_path)
{	
	vector<int> correct_lines;
	vector<int> wrong_lines;
	ifstream problem_file(problem_file_path,std::ios::binary);
	ifstream answer_file(answer_file_path,std::ios::binary);

	if (!problem_file.is_open()) {
		printf("Error：cannot open the file %s！\n", problem_file_path.c_str());
		return;
	}

	if (!answer_file.is_open()) {
		problem_file.close();
		printf("Error：cannot open the file %s！\n", answer_file_path.c_str());
		return;
	}

	string problem_str, answer_str;
	int line = 0;

	while (getline(problem_file, problem_str) && getline(answer_file, answer_str)) {
		line++;
		if (verify_oneline(problem_str, answer_str))
		{
			correct_lines.push_back(line);
		}
		else
		{
			wrong_lines.push_back(line);
		}
	}

	problem_file.close();
	answer_file.close();

	const string result_file_path = "Grade.txt";
	ofstream result_file(result_file_path,std::ios::binary);

	if (!result_file.is_open()) {
		printf("cannot write to the file Grade.txt\n");
		return;
	}

	result_file << "Correct: " << correct_lines.size() << " (";

	for (auto it = correct_lines.begin(); it != correct_lines.end(); ++it) {
		if (it == correct_lines.end() - 1) {
			result_file << *it;
		}
		else 
		{
			result_file << *it << ", ";
		}
	}

	result_file << ")" << endl;

	result_file << "Wrong: " << wrong_lines.size() << " (";

	for (auto it = wrong_lines.begin(); it != wrong_lines.end(); ++it) {
		if (it == wrong_lines.end() - 1) {
			result_file << *it;
		}
		else
		{
			result_file << *it << ", ";
		}
	}

	result_file << ")" << endl;
	result_file.close();
}

