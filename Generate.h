#ifndef __GENERATE_H__
#define __GENERATE_H__

Fraction generate_postive_number(int range);
Expression* generate_expression(int& current_op_count, int range);
void generate_problems(int count, int range);
#endif