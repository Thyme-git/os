#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* dir_name, char* file_name)
{
    char buf[128];
    struct stat st;
    struct dirent de;
    int dir_fd = open(dir_name, 0);
    if (dir_fd < 0)
    {
        fprintf(2, "find: cannot open %s\n", dir_name);
        return;
    }
    if (fstat(dir_fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", dir_name);
        close(dir_fd);
        return;
    }

    if (st.type != T_DIR)
    {
        fprintf(2, "find: %s is not a directory\n", dir_name);
        close(dir_fd);
        return;
    }

    
    strcpy(buf, dir_name);
    int len = strlen(dir_name);
    buf[len] = '/';
    while (read(dir_fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0 || strcmp(de.name, ".") == 0 ||  strcmp(de.name, "..") == 0)
        {
            continue;
        }
        strcpy(buf+len+1, de.name);
        int fd = open(buf, 0);
        if (fd < 0)
        {
            fprintf(2, "find: cannot open %s\n", buf);
            close(dir_fd);
            return;
        }
        if (fstat(fd, &st) < 0)
        {
            fprintf(2, "find: cannot stat %s\n", buf);
            close(fd);
            close(dir_fd);
            return;
        }
        if (st.type == T_DIR)
        {
            find(buf, file_name);
        }
        if (strcmp(de.name, file_name) == 0)
        {
            printf("%s\n", buf);
        }
        close(fd);
    }
    close(dir_fd);
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(2, "find should take 2 required arguments\n");
        fprintf(2, "[Usage]\n");
        fprintf(2, "\tfind dir file-name\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}