#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libunix.h"

// read entire file into buffer.  return it.   zero pads to a 
// multiple of 4.
//
// make sure to cleanup!
uint8_t *read_file(unsigned *size, const char *name) {
    uint8_t *buf = 0;

    struct stat sb;
    if (stat(name, &sb) == -1) {
        panic("stat() failed\n");
    }

    int fd;
    fd = open(name, O_RDONLY);
    if (fd < 0) {
        panic("could not open file '%s'\n", name);
    }

    *size = sb.st_size;
    size_t nbytes  = roundup(sb.st_size, 4);

    buf = (uint8_t *)calloc(nbytes, 1);
    if (buf == NULL) {
        panic("calloc() failed\n");
    }

    ssize_t n;
    n = read(fd, buf, sb.st_size);
    if (n < 0) {
        panic("read() failed\n");
    }

    if (n != sb.st_size) {
        panic("read() read not the whole file\n");
    }

    close(fd);

    return buf;
}
