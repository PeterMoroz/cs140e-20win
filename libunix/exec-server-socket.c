#include <unistd.h>
#include <sys/socket.h>

#include "libunix.h"

// fork exec server, create a socket for comparison and dup it to the <to_fd>
int exec_server_socket(int *pid, char **argv, int to_fd) {

    debug("going to fork/exec: %s ", argv[0]);
    for(int i = 1; argv[i]; i++)
        fprintf(stderr, " argv[%d]=<%s> ", i, argv[i]);
    fprintf(stderr, "\n");

    int sock[2] = { -1 };
    if (socketpair(PF_LOCAL, SOCK_STREAM, 0, sock) < 0)
        sys_die(socketpair, failed);

    if (dup2(sock[1], to_fd) < 0)
        sys_die(dup2, failed);

    if (close(sock[1]) < 0)
        sys_die(close, failed);

    if ((*pid = fork()) < 0)
        sys_die(fork, failed);

    if (!*pid) {
        if (execvp(argv[0], argv) < 0)
            sys_die(execvp, failed);
    }

    return sock[0];
}
