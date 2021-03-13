#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXLINE 1024
#define STDIN 0
#define STDOUT 0

// 感觉有点问题，应该每次读入一个byte，遇到\n就去执行一次exec
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "Usage: xargs <command>\n");
        exit(1);
    }

    char buf[MAXLINE];      //stores stdin
    char *_argv[MAXARG];     //stores argv for exec in child process
    char *cmd = argv[1];    //command

    int n, i, args_index = 0;   // args_index is the lenth of _argv

    // Put arguments from xargs commad into _argv
    for (i = 1; i < argc; i++)
    {
        _argv[args_index++] = argv[i];
    }

    while ((n = read(STDIN, buf, MAXLINE)) > 0)   // Read all stdin into buf
    {
        printf("test\n");
        if (fork() == 0) // child process
        {
            char *arg = (char*) malloc(sizeof(buf));
            int index = 0;
            for (i = 0; i < n; i++)
            {
                if (buf[i] == ' ' || buf[i] == '\n')
                {
                    // printf("%d\n", args_index);
                    _argv[args_index++] = arg;      // Attach arg from stdin to _argv
                    index = 0;
                    arg = (char*) malloc(sizeof(buf));
                }
                else arg[index++] = buf[i];
            }
            arg[index] = 0;
            _argv[args_index] = 0;
            printf("%d\n", args_index);
            exec(cmd, _argv);
        }
        else wait(0);
    }
    exit(0);  
}
