#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        write(1, "sleep should take one required argument\n", 40);
        exit(1);
    }

    // if (argc > 2)
    // {
    //     write(1, "too many arguments\n", 19);
    //     exit(1);
    // }

    int n = atoi(argv[1]);
    sleep(n);

    exit(0);
}