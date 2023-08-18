#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ_END 0
#define WRITE_END 1

int main()
{
    int down_pipefd[2];
    int up_pipefd[2];
    char msg = 't';
    
    pipe(down_pipefd);
    pipe(up_pipefd); 

    int pid = fork();
    if (pid == 0)
    {
        close(down_pipefd[WRITE_END]);
        close(up_pipefd[READ_END]);
        read(down_pipefd[READ_END], &msg, 1);
        printf("%d: received ping\n", getpid());
        write(up_pipefd[WRITE_END], &msg, 1);
        close(down_pipefd[READ_END]);
        close(up_pipefd[WRITE_END]);
    }else if (pid > 0)
    {
        close(down_pipefd[READ_END]);
        close(up_pipefd[WRITE_END]);
        write(down_pipefd[WRITE_END], &msg, 1);
        read(up_pipefd[READ_END], &msg, 1);
        printf("%d: received pong\n", getpid());
        close(down_pipefd[WRITE_END]);
        close(up_pipefd[READ_END]);
    }else
    {
        write(1, "fork error\n", 11);
        exit(1);
    }

    exit(0);
}