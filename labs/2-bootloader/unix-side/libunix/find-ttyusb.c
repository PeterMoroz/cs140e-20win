// engler, cs140e.
#include <assert.h>
#include <fcntl.h>
#include <string.h>

#include "libunix.h"

#define _SVID_SOURCE
#include <dirent.h>
static const char *ttyusb_prefixes[] = {
	"ttyUSB",	// linux
	"cu.SLAB_USB", // mac os
	0
};

static int filter(const struct dirent *d) {
    // scan through the prefixes, returning 1 when you find a match.
    // 0 if there is no match.
    return strncmp(d->d_name, "ttyUSB", strlen("ttyUSB")) == 0 ? 1 : 0;
}

// find the TTY-usb device (if any) by using <scandir> to search for
// a device with a prefix given by <ttyusb_prefixes> in /dev
// returns:
//  - device name.
// panic's if 0 or more than 1.
//
char *find_ttyusb(void) {
    char *p;

    // use <alphasort> in <scandir>
    // return a malloc'd name so doesn't corrupt.
    struct dirent **namelist;
    int n;

    n = scandir("/dev", &namelist, filter, alphasort);
    if (n == -1) {
        panic("scandir() failed\n");
    }

    if (n == 0) {
        panic("no ttyUSB device(s)\n");
    }

    if (n > 1) {
        panic("too many ttyUSB devices\n");
    }

    p = namelist[0]->d_name;

    static char name[261] = { 0 };
    sprintf(name, "/dev/%s", namelist[0]->d_name);

    free(namelist[0]);
    free(namelist);

    p = name;

    return p;
}
