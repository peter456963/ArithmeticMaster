#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <string>
#include "Fraction.h"

enum ExpressionType
{ 
    EXPRESSION_TYPE_NUMBER, 
    EXPRESSION_TYPE_ADD, 
    EXPRESSION_TYPE_SUB, 
    EXPRESSION_TYPE_MULTIPLY, 
    EXPRESSION_TYPE_DIVIDE
};

struct Expression
{
    ExpressionType type;
	Expression* left;
	Expression* right;
	Fraction number;
};

Expression* expression_create();
void expression_destroy(Expression* expr);
Fraction expression_compute(Expression* expr);
std::string expression_toString(Expression* expr);
bool expression_equals(Expression *a,Expression *b);

#endif