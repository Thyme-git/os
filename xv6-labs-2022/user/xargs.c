#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int readline(char buf[])
{
    int i = 0;
    while ((read(0, buf+i, 1)) > 0 && buf[i++] != '\n');
    
    if (i)
        buf[--i] = '\0';
    
    return i;
}

int parse_args(char buf[], char* argv[])
{

    int hold = 0;
    int cnt = 0;
    for (int i = 0; buf[i] != '\0'; ++i)
    {
        if (buf[i] != ' ' && hold == 0)
        {
            argv[cnt++] = buf+i;
            if (cnt > MAXARG)
            {
                fprintf(2, "xargs: reach maximum arguments count\n");
                exit(1);
            }
            hold = 1;
        }
        if (buf[i] == ' ')
        {
            buf[i] = '\0';
            hold = 0;
        }
    }
    argv[cnt] = 0;
    return cnt+1;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(2, "xargs takes at least 1 argument\n");
        exit(1);
    }

    char* argv_to_pass[MAXARG];
    char buf[128];

    for (int i = 0; i < argc-1; ++i)
    {
        argv_to_pass[i] = argv[i+1];
    }

    while (readline(buf) > 0)
    {
        parse_args(buf, argv_to_pass+argc-1);

        int pid = fork();
        if (pid > 0)
        {
            wait((int*)0);
        }else if (pid == 0)
        {
            exec(argv_to_pass[0], argv_to_pass);
            fprintf(2, "exec %s fail\n", argv_to_pass[0]);
            exit(1);
        }else
        {
            fprintf(2, "fork error\n");
            exit(1);
        }
    }

    exit(0);
}