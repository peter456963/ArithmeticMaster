#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_PROBLEMS 10000
#define MAX_EXPRESSION_LENGTH 100

typedef struct {
    int numerator;  // 分子
    int denominator;  // 分母
} Fraction;

// 函数原型
Fraction add_fractions(Fraction a, Fraction b);  //加法
Fraction subtract_fractions(Fraction a, Fraction b);  //减法
Fraction multiply_fractions(Fraction a, Fraction b);  //乘法
Fraction divide_fractions(Fraction a, Fraction b);  //除法
Fraction generate_random_fraction(int range);  //确定题目范围
int gcd(int a, int b);  //计算最大公约数
void simplify(Fraction *f);  //化简分数
void print_fraction(Fraction f);  //打印分数
void generate_problem(int n, int r, FILE *exerciseFile, FILE *answerFile, char expressions[MAX_PROBLEMS][MAX_EXPRESSION_LENGTH], int *problem_count);  //生成题目
Fraction calculate_answer(char *expression);  //计算答案
void standardize_expression(char *expression); //格式化表达式,便于检查是否重复
int is_duplicate(char *expression, char expressions[MAX_PROBLEMS][MAX_EXPRESSION_LENGTH], int count);  //检查题目是否重复

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: Myapp.exe -n <number> -r <range>\n");
        return 1;
    }

    int num_problems = 0;
    int range = 0;

    // 解析命令行参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            num_problems = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-r") == 0) {
            range = atoi(argv[i + 1]) - 1;
        }
    }

    if (num_problems <= 0 || range <= 0) {
        printf("Invalid input for number of problems or range.\n");
        return 1;
    }

    // 打开文件进行输出
    FILE *exerciseFile = fopen("Exercises.txt", "w");
    FILE *answerFile = fopen("Answers.txt", "w");

    if (!exerciseFile || !answerFile) {
        printf("Error opening files.\n");
        return 1;
    }

    // 随机数种子
    srand(time(NULL));

    char expressions[MAX_PROBLEMS][MAX_EXPRESSION_LENGTH];
    int problem_count = 0;

    // 生成题目
    while (problem_count < num_problems) {
        generate_problem(num_problems, range, exerciseFile, answerFile, expressions, &problem_count);
    }

    fclose(exerciseFile);
    fclose(answerFile);

    printf("Problems generated successfully.\n");
    return 0;
}

Fraction add_fractions(Fraction a, Fraction b) {
    Fraction result;
    result.numerator = a.numerator * b.denominator + b.numerator * a.denominator;
    result.denominator = a.denominator * b.denominator;
    simplify(&result);
    return result;
}

Fraction subtract_fractions(Fraction a, Fraction b) {
    Fraction result;
    result.numerator = a.numerator * b.denominator - b.numerator * a.denominator;
    result.denominator = a.denominator * b.denominator;
    simplify(&result);
    return result;
}

Fraction multiply_fractions(Fraction a, Fraction b) {
    Fraction result;
    result.numerator = a.numerator * b.numerator;
    result.denominator = a.denominator * b.denominator;
    simplify(&result);
    return result;
}

Fraction divide_fractions(Fraction a, Fraction b) {
    Fraction result;
    result.numerator = a.numerator * b.denominator;
    result.denominator = a.denominator * b.numerator;
    simplify(&result);
    return result;
}

Fraction generate_random_fraction(int range) {
    Fraction f;
    f.numerator = rand() % range + 1;
    f.denominator = rand() % (range - 1) + 2;  //分母若为1则是普通整数
    simplify(&f);
    return f;
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void simplify(Fraction *f) {
    int common_divisor = gcd(f->numerator, f->denominator);
    //分子、分母同时除以最大公约数
    f->numerator /= common_divisor;
    f->denominator /= common_divisor;
}

void print_fraction(Fraction f) {
    if (f.denominator == 1) {
        printf("%d", f.numerator);
    } else {
        printf("%d/%d", f.numerator, f.denominator);
    }
}

// 生成数学问题和答案
// 参数n: 需要生成的问题数量
// 参数r: 随机数范围
// exerciseFile: 保存题目的文件指针
// answerFile: 保存答案的文件指针
// expressions: 保存生成的表达式数组
// problem_count: 指向当前已生成问题数量的指针
void generate_problem(int n, int r, FILE *exerciseFile, FILE *answerFile, char expressions[MAX_PROBLEMS][MAX_EXPRESSION_LENGTH], int *problem_count) {
    char expression[MAX_EXPRESSION_LENGTH];
    char standardized_expression[MAX_EXPRESSION_LENGTH];
    Fraction result;
    int num1, operator;

    // 随机选择运算符数量（1到3个）
    int num_operators = rand() % 3 + 1;

    // 初始化表达式和结果
    num1 = rand() % r + 1;
    sprintf(expression, "%d", num1);
    result = (Fraction){num1, 1};

    for (int i = 0; i < num_operators; i++) {
        num1 = rand() % r + 1;
        operator = rand() % 4; // 0: +, 1: -, 2: *, 3: /

        switch (operator) {
            case 0:
                strcat(expression, " + ");
                result = add_fractions(result, (Fraction){num1, 1});
                break;
            case 1:
                // 确保结果为正数
                if (result.numerator >= num1) {
                    strcat(expression, " - ");
                    result = subtract_fractions(result, (Fraction){num1, 1});
                } else {
                    // 如果结果为负数，重新生成题目
                    generate_problem(n, r, exerciseFile, answerFile, expressions, problem_count);
                    return;
                }
                break;
            case 2:
                strcat(expression, " * ");
                result = multiply_fractions(result, (Fraction){num1, 1});
                break;
            case 3:
                if (num1 != 0) {
                    strcat(expression, " / ");
                    result = divide_fractions(result, (Fraction){num1, 1});
                } else {
                    // 如果除数为0，重新生成题目
                    generate_problem(n, r, exerciseFile, answerFile, expressions, problem_count);
                    return;
                }
                break;
        }

        // 确保结果为正数
        if (result.numerator < 0) {
            // 如果结果为负数，重新生成题目
            generate_problem(n, r, exerciseFile, answerFile, expressions, problem_count);
            return;
        }

        // 将数字添加到表达式中
        char num_str[10];
        sprintf(num_str, "%d", num1);
        strcat(expression, num_str);
    }

    // 复制原表达式到标准化表达式变量
    strcpy(standardized_expression, expression);

    // 标准化表达式
    standardize_expression(standardized_expression);

    // 检查表达式是否重复
    if (is_duplicate(standardized_expression, expressions, *problem_count)) {
        return; // 如果重复，跳过当前题目
    }

    // 存储题目和答案
    strcpy(expressions[*problem_count], standardized_expression);
    (*problem_count)++;

    fprintf(exerciseFile, "%s =\n", expression); // 使用原表达式输出题目
    printf("Generated problem: %s = ", expression); // 调试信息
    print_fraction(result);
    printf("\n"); // 调试信息
    fprintf(answerFile, "%d\n", result.numerator); // 简化版答案输出
}

void standardize_expression(char *expression) {
    // 去掉空格
    char *src = expression, *dst = expression;
    while (*src) {
        if (!isspace((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';

    // 对加法和乘法进行排序
    char temp[MAX_EXPRESSION_LENGTH];
    strcpy(temp, expression);
    char *token = strtok(temp, "+*");
    int numbers[100];
    int num_count = 0;
    while (token != NULL) {
        numbers[num_count++] = atoi(token);
        token = strtok(NULL, "+*");
    }

    // 对数字进行排序
    for (int i = 0; i < num_count - 1; i++) {
        for (int j = i + 1; j < num_count; j++) {
            if (numbers[i] > numbers[j]) {
                int temp_num = numbers[i];
                numbers[i] = numbers[j];
                numbers[j] = temp_num;
            }
        }
    }

    // 重新构建表达式
    char new_expression[MAX_EXPRESSION_LENGTH];
    new_expression[0] = '\0';
    for (int i = 0; i < num_count; i++) {
        char num_str[10];
        sprintf(num_str, "%d", numbers[i]);
        strcat(new_expression, num_str);
        if (i < num_count - 1) {
            strcat(new_expression, "+");
        }
    }

    strcpy(expression, new_expression);
}

int is_duplicate(char *expression, char expressions[MAX_PROBLEMS][MAX_EXPRESSION_LENGTH], int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(expression, expressions[i]) == 0) {
            return 1; // 表达式重复
        }
    }
    return 0; // 没有重复
}
