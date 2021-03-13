#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READEND 0
#define WRITEEND 1

int main(int argc, char const *argv[])
{
    int numbers[36];
    int count = 0;
    int p[2], pid, i;

    // initialize input
    for (i = 2; i <= 35; i++)
    {
        numbers[count++] = i; 
    }

    while (count > 0)
    {
        pipe(p);
        if ((pid = fork()) < 0)
        {
            fprintf(2, "Fork failed!");
            exit(1);
        }
        // Child process reads data.
        // A child process will eventually become a parent process, and wirtes data to its child. 
        else if (pid == 0)
        {
            int number, prime = 0;
            close(p[WRITEEND]);
            count = -1;
            // Will block until parent process writes data. 
            while (read(p[READEND], &number, sizeof(number)) != 0)
            {
                if (count == -1)
                {
                    prime = number;
                    count ++;
                }
                else
                {
                    if (number % prime != 0)
                    {
                        numbers[count++] = number;
                    }
                }
            }
            // printf("child pid: %d count: %d\n", getpid(), count);
            printf("prime %d\n", prime);
            close(p[READEND]);
        }
        // Parent process starts here. Write data to child. 
        else
        {
            close(p[READEND]);
            for (i = 0; i < count; i++)
            {
                write(p[WRITEEND], &numbers[i], sizeof(numbers[i]));
            }
            // printf("parent pid: %d count: %d\n", getpid(), count);
            close(p[WRITEEND]);
            // Parent process should wait until its child process exit. 
            wait(0);
            // This break will end the while loop to fork more process.
            // printf("parent pid: %d wait finished\n", getpid());
            break;
        }
    }
    exit(0);
}
