#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ_END 0
#define WRITE_END 1
#define RANGE_MAX 35

void pass_number(int read_end_fd)
{
    int base_number;
    int number;

    if (read(read_end_fd, &base_number, sizeof(number)) == 0)
    {
        return;
    }
    printf("prime %d\n", base_number);

    int pipefd[2];
    pipe(pipefd);

    int pid = fork();
    if (pid > 0)
    {
        close(pipefd[READ_END]);
        while (read(read_end_fd, &number, sizeof(number)))
        {
            if (number % base_number)
            {
                write(pipefd[WRITE_END], &number, sizeof(number));
            }
        }
        close(pipefd[WRITE_END]);
        while(wait((int*)0) != -1);
    }else if (pid == 0)
    {
        close(pipefd[WRITE_END]);
        pass_number(pipefd[READ_END]);
        close(pipefd[READ_END]);
    }else
    {
        fprintf(2, "fork error\n");
        exit(1);
    }
}

int main()
{
    int pipefd[2];
    pipe(pipefd);

    int pid = fork();
    if (pid > 0)
    {
        close(pipefd[READ_END]);
        for (int i = 2; i <= RANGE_MAX; ++i)
        {
            write(pipefd[WRITE_END], &i, sizeof(i));
        }
        close(pipefd[WRITE_END]);
        while(wait((int*)0) != -1);
    }else if (pid == 0)
    {
        close(pipefd[WRITE_END]);
        pass_number(pipefd[READ_END]);
        close(pipefd[READ_END]);
    }else
    {
        fprintf(2, "fork error\n");
        exit(1);
    }


    exit(0);
}