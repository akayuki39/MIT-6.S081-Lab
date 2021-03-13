#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
    int child_to_parent[2], parent_to_child[2];   // child_to_parent[0] and child_to_parent[1] are file descriptors for read and write
    
    pipe(child_to_parent);
    pipe(parent_to_child);

    char buffer[4];
    int pid;

    if ((pid = fork()) < 0)
    {
        fprintf(2, "Fork Failed!\n");
        exit(1);
    }
    if (pid == 0)   // child process
    {
        close(child_to_parent[0]);
        close(parent_to_child[1]);
        read(parent_to_child[0], buffer, 4);
        printf("%d: received %s\n", getpid(), buffer);
        write(child_to_parent[1], "pong", 4);
        close(child_to_parent[1]);
        close(parent_to_child[0]);
    }
    else    // parent process
    {
        close(parent_to_child[0]);
        close(child_to_parent[1]);
        // Write to pipe, the read() in child process will receive data and print. 
        // One of the write() should be ahead of read(), otherwise the read() will block. 
        write(parent_to_child[1], "ping", 4);
        read(child_to_parent[0], buffer, 4);
        printf("%d: received %s\n", getpid(), buffer);
        close(parent_to_child[1]);
        close(child_to_parent[0]);
    }
    exit(0);
}
