#include <string>
#include <sstream>
#include "Expression.h"

using namespace std;

int precedence(ExpressionType type) {
    switch (type) {
    case EXPRESSION_TYPE_NUMBER: return 3; 
    case EXPRESSION_TYPE_MULTIPLY:
    case EXPRESSION_TYPE_DIVIDE: return 2;
    case EXPRESSION_TYPE_ADD:
    case EXPRESSION_TYPE_SUB: return 1;
    default: return 0;
    }
}

Expression* expression_create()
{
    Expression* expr = (Expression*)malloc(sizeof(Expression));
    memset(expr, 0, sizeof(Expression));
    return expr;
}

void expression_destroy(Expression *expr)
{
    if (expr->type != EXPRESSION_TYPE_NUMBER)
    { 
        expression_destroy(expr->left);
        expression_destroy(expr->right);
    }

    free(expr);
}

Fraction expression_compute(Expression * expr) {
    if (expr->type == EXPRESSION_TYPE_NUMBER)
    {
        return expr->number;

    }

    Fraction left = expression_compute(expr->left);
    Fraction right = expression_compute(expr->right);

    switch (expr->type) {
    case EXPRESSION_TYPE_ADD: return fraction_add(left, right);
    case EXPRESSION_TYPE_SUB: return fraction_subtract(left, right);
    case EXPRESSION_TYPE_MULTIPLY: return fraction_multiply(left, right);
    case EXPRESSION_TYPE_DIVIDE: return fraction_divide(left, right);
	default: printf("未知的表达式类型\n");fprintf(stderr,"未知的表达式类型\n"); return left;
    }
}

string expression_toString(Expression *expr) {
    if (expr->type == EXPRESSION_TYPE_NUMBER) {
        return fraction_toString(expr->number);
    }

    string left = expression_toString(expr->left);
    string right = expression_toString(expr->right);

    string op;
    switch (expr->type)
    {
    case EXPRESSION_TYPE_ADD: op = "+"; break;
    case EXPRESSION_TYPE_SUB: op = "-"; break;
    case EXPRESSION_TYPE_MULTIPLY: op = "×"; break;
    case EXPRESSION_TYPE_DIVIDE: op = "÷"; break;
    default: op = '?'; fprintf(stderr, "未知的表达式类型\n");;
    }

    stringstream ss;

    // 若父节点的操作优先级高于左子节点，则必须加括号
    if (precedence(expr->type) > precedence(expr->left->type))
    {
        ss << "(" << left << ")";
    }
    else
    {
        ss << left;
    }
    

    ss << " " << op << " ";

    // 若父节点的操作优先级大于等于右子节点，则必须加括号
    if (precedence(expr->type) >= precedence(expr->right->type))
    {
        ss << "(" << right << ")";
    }
    else
    {
        ss << right;
    }

    return ss.str();
}