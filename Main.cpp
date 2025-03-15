#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _WIN32
#include <Windows.h>
#endif
#include "Expression.h"
#include "Fraction.h"
#include "Generate.h"
#include "Verify.h"
#include "Main.h"


int parse_arguments(int argc, char **argv, CmdArgs *args)
{
    memset(args, 0, sizeof(CmdArgs));

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-n") == 0)
        {
            if (i + 1 >= argc)
            {
				printf("Error：missing count after -n \n");
                return -1;
            }
            args->problem_count = atoi(argv[i + 1]);
            args->generate_mode = 1;
            i++;
        }
        else if (strcmp(argv[i], "-r") == 0)
        {
            if (i + 1 >= argc)
            {
				printf("Error：missing range after -r\n");
                return -1;
            }
            args->range = atoi(argv[i + 1]);
            args->generate_mode = 1;
            i++;
        }
        else if (strcmp(argv[i], "-e") == 0)
        {
            if (i + 1 >= argc)
            {
				printf("Error：missing exercise file after -e\n");
                return -1;
            }
            args->exercise_file_path = argv[i + 1];
            args->verify_mode = 1;
            i++;
        }
        else if (strcmp(argv[i], "-a") == 0)
        {
            if (i + 1 >= argc)
            {
				printf("Error：missing the answer file after -a\n");
                return -1;
            }
            args->answer_file_path = argv[i + 1];
            args->verify_mode = 1;
            i++;
        }
        else
        {
			printf("Error：unnknow argument '%s'\n", argv[i]);
            return -1;
        }
    }

    if (args->generate_mode && args->verify_mode)
    {
        printf("Error：cannot use the generate function and the verify function at the same time\n");
        return -1;
    }

    if (args->generate_mode)
    {
        if (args->range <= 0)
        {
			printf("Error：missing the range\n");
            printf("Usage: %s -n <count of problems> -r <range>\n", argv[0]);
            return -1;
        }
        if (args->problem_count <= 0)
        {
			printf("Error：missing the count of problems\n");
            printf("Usage: %s -n <count of problems> -r <range>\n", argv[0]);
            return -1;
        }
    }

    if (args->verify_mode)
    {
        if (!args->exercise_file_path || !args->answer_file_path)
        {
			printf("Error：missing the exercisefile or answerfile\n");
            printf("Usage: %s -e <exercisefile>.txt -a <answerfile>.txt\n", argv[0]);
            return -1;
        }
    }

    if (!args->generate_mode && !args->verify_mode)
    {
        printf("Usage:\n");
		printf("1. Generate: %s -n <count of problems> -r <range>\n",argv[0]);
        printf("2. Verify: %s -e <exercisefile>.txt -a <answerfile>.txt\n", argv[0]);
        return -1;
    }

    return 0;
}


int main(int argc, char **argv)
{
    CmdArgs args;

#ifdef _WIN32
	SetConsoleOutputCP(65001);  // 设置控制台编码UTF-8
#endif

    if (parse_arguments(argc, argv, &args) != 0)
    {
        return -1;
    }

    if (args.generate_mode)
    {
        generate_problems(args.problem_count, args.range);
    }
    else if (args.exercise_file_path)
    {
        verify(args.exercise_file_path, args.answer_file_path);
    }

    return 0;
}