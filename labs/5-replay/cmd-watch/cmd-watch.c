// simple program to run a command when any file that is "interesting" in a directory
// changes.
// e.g., 
//      cmd-watch make
// will run make at each change.
//
// This should use the scandir similar to how you did `find_ttyusb`
//
// key part will be implementing two small helper functions (useful-examples/ will be 
// helpful):
//  - static int pid_clean_exit(int pid);
//  - static void run(char *argv[]);
//
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "libunix.h"

#define _SVID_SOURCE
#include <dirent.h>

// scan the files in "./" (you can extend this) for those
// that match the suffixes in <suffixes> and check  if any
// have changed since the last time.
int check_activity(void) {
    char *suffixes[] = { ".c", ".h", ".S", "Makefile", 0 };
    const char *dirname = ".";
    int changed_p = 0;

    static time_t last_mtime;   // store last modification time.

    DIR *dir = opendir(dirname);
    if (dir == NULL)
        panic("opendir() failed.\n");

    struct dirent *entry = NULL;
    struct stat fstat;
    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        const char *dot = strrchr(entry->d_name, '.');
        if (dot == NULL)
            dot = entry->d_name;
        char **s = suffixes;
        
        for (; *s; s++) {
            if (strcmp(*s, dot) == 0)
                break;
        }

        if (*s == 0)
            continue;

        if (stat(entry->d_name, &fstat) < 0)
            panic("stat() failed.\n");

        if (fstat.st_mtime > last_mtime) {
            last_mtime = fstat.st_mtime;
            changed_p = 1;
        }
    }

    // return 1 if anything that matched <suffixes>
    return changed_p;
}

// synchronously wait for <pid> to exit.  returns 1 if it exited
// cleanly (via exit(0)), 0 otherwise.
static int pid_clean_exit(int pid) {
    int status = -1;
    if (waitpid(pid, &status, 0) < 0)
        panic("waitpid() failed.\n");
    return WIFEXITED(status) != 0 ? 1 : 0;
}

// simple helper to print null terminated vector of strings.
static void print_argv(char *argv[]) {
  assert(argv[0]);

	fprintf(stderr, "about to exec =<%s ", argv[0]);
	for(int i =1; argv[i]; i++)
		fprintf(stderr, " %s", argv[i]);
	fprintf(stderr, ">\n");
}


// fork/exec <argv> and wait for the result: print an error
// and exit if the kid crashed or exited with an error (a non-zero
// exit code).
static void run(char *argv[]) {
    assert(argv[0]);
    pid_t pid = fork();
    if (pid < 0)
        panic("fork() failed.\n");

    if (pid == 0) {
        if (execvp(argv[0], argv) < 0)
            panic("execvp() failed.\n");
    } else {
        if (pid_clean_exit(pid) == 0)
            die("the child process terminated ubnormally.\n");
    }
}

int main(int argc, char *argv[]) {
    if(argc < 2)
        die("cmd-watch: not enough arguments\n");

    while (1) {
        if (usleep(250000) < 0)
            panic("usleep() failed.\n");

        if (check_activity()) {
            run(argv + 1);
        }
    }
    
    return 0;
}
