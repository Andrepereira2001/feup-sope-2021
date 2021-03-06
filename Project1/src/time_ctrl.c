#include "../include/time_ctrl.h"

#include <errno.h> // errno
#include <stdio.h> // BUFSIZ
#include <string.h> // strlen()
#include <sys/stat.h> // stat()
#include <time.h> // clock_gettime()
#include <unistd.h> // getpgrp()

#include "../include/utils.h" // convertIntegerToString()

int getStartTime(struct timespec *dest) {
    if (dest == NULL) return -1;
    char path[BUFSIZ] = "/proc/";
    if (convertIntegerToString(getpgrp(), path + strlen(path), sizeof(path) - strlen(path))) return -1;
    struct stat buf;
    errno = 0;
    // Try to read process file's info
    if (stat(path, &buf) == -1) return -1;
    *dest = buf.st_atim; // Store the result
    return 0;
}

int getMillisecondsElapsed(void) {
    struct timespec now, then;
    if (getStartTime(&then)) return -1; // Get first process' start time
    if (clock_gettime(CLOCK_REALTIME, &now) == -1) return -1; // Get current time
    return (int) ((double) (now.tv_sec - then.tv_sec) * 1e3 + (double) (now.tv_nsec - then.tv_nsec) * 1e-6);
}
