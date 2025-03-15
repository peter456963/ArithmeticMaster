#ifndef  __MAIN_H__
#define __MAIN_H__

struct CmdArgs
{
    int generate_mode;
    int verify_mode;
    int problem_count;
    int range;
    char* exercise_file_path;
    char* answer_file_path;
};

#endif 