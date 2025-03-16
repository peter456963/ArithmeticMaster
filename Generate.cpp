#define _CRT_SECURE_NO_WARNINGS
#include "Fraction.h"
#include "Expression.h"
#include <random>

using namespace std;

random_device generate_rd;
mt19937 generate_gen(generate_rd());

Fraction generate_postive_number(int range) {
	Fraction f;
	const int64_t factor = 5;
	uniform_int_distribution<int64_t> dis_int1(2,range * factor);
	uniform_int_distribution<int> dis_int2(1,3);
	f.denominator = dis_int1(generate_gen);
	int choice = dis_int2(generate_gen);

	if(choice == 1 || range == 1) // 生成真分数
	{
		uniform_int_distribution<int64_t> dis_int2(1,f.denominator - 1);
		f.numerator = dis_int2(generate_gen);
	} 
	else if(choice == 2) // 生成整数
	{
		uniform_int_distribution<int64_t> dis_int2(1,range - 1);
		f.numerator = f.denominator * dis_int2(generate_gen) ;
	} 
	else // 生成带分数
	{
		uniform_int_distribution<int64_t> dis_int2(f.denominator + 1,f.denominator * range - 1);
		f.numerator = dis_int2(generate_gen) ;
	}

	fraction_simplify(&f);

	return f;
}

Expression * generate_expression(int &current_op_count,int range) {
	const int max_op_count = 3;
	uniform_int_distribution<> dis_int(1,4);
	Expression* expr = expression_create();

	// 操作符达到上限或随机到不再生成操作符

	if(current_op_count >= max_op_count || dis_int(generate_gen) % 2 == 0) {
		expr->type = EXPRESSION_TYPE_NUMBER;
		expr->number = generate_postive_number(range);
	} 
	else
	{
		const ExpressionType types[4] = {EXPRESSION_TYPE_ADD,EXPRESSION_TYPE_SUB,EXPRESSION_TYPE_MULTIPLY,EXPRESSION_TYPE_DIVIDE};

		current_op_count++;

		expr->type = types[dis_int(generate_gen) - 1];
		expr->left = generate_expression(current_op_count,range);
		expr->right = generate_expression(current_op_count,range);

		// 防止减法运算产生负数
		if(expr->type == EXPRESSION_TYPE_SUB && fraction_compare(expression_compute(expr->left),expression_compute(expr->right)) < 0)
		{
			swap(expr->left,expr->right);
		} 
		else if(expr->type == EXPRESSION_TYPE_DIVIDE) // 防止产生非真分数，即分子大于等于分母
		{
			if(expression_compute(expr->right).numerator == 0) // 防止除数为0
			{
				Fraction right;

				right.denominator = right.numerator = 1;

				expression_destroy(expr->right);
				expr->right = expression_create();
				expr->right->type = EXPRESSION_TYPE_NUMBER;
				expr->right->number = right;
			}

			Fraction result = expression_compute(expr);
			// 若等于1，left直接替换为更小的数
			if(result.numerator == result.denominator)
			{
				Fraction left = expression_compute(expr->left);
				left.denominator++;

				expression_destroy(expr->left);
				expr->left = expression_create();
				expr->left->type = EXPRESSION_TYPE_NUMBER;
				expr->left->number = left;
			} 
			else if(result.numerator > result.denominator) // 若大于1，则直接交换即可
			{
				swap(expr->left,expr->right);
			}
		}
	}

	return expr;
}

void generate_problems(int count,int range)
{
	FILE *exercises_file = fopen("Exercises.txt","wb");
	FILE *answer_file = fopen("Answers.txt","wb");

	if(!exercises_file) {
		printf("Error：cannot open Exercises.txt\n");
		return;
	}

	if(!answer_file) {
		fclose(exercises_file);
		printf("Error：cannot open Answers.txt\n");
		return;
	}

	vector<Expression *> exprs;

	for(int i = 1; i <= count ;i++)
	{
		int op_count = 0;
		
		Expression * expr = generate_expression(op_count,range);

		// 防止出现重复的题目
		for(int j = 0; j < i - 1; j++)
		{
			if(expression_equals(exprs[j], expr))
			{
				expression_destroy(expr);
				op_count = 0;
				expr = generate_expression(op_count,range);
				j = -1;
			}
		}

		exprs.push_back(expr);

		fprintf(exercises_file,"%d. %s =\n",i,expression_toString(expr).c_str());
		fprintf(answer_file,"%d. %s\n",i,fraction_toString(expression_compute(expr)).c_str());

	}

	for(int i = 0; i < count; i++) 
	{
		expression_destroy(exprs[i]);
	}

	fclose(exercises_file);
	fclose(answer_file);
}