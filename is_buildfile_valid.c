#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "is_buildfile_valid.h"
#include "output_styles.h"

bool is_buildfile_valid(const char *buildfile) {
    bool is_valid = true;
    auto error = "does not exist";
    struct stat buildfile_stat;
    if (access(buildfile, F_OK) != 0) {
        is_valid = false;
    } else if (access(buildfile, R_OK) != 0) {
        is_valid = false;
        error = "is not readable";
    } else if (stat(buildfile, &buildfile_stat) != 0) {
        is_valid = false;
        error = "can't be opened for reading";
    } else if (!S_ISREG(buildfile_stat.st_mode)) {
        is_valid = false;
        error = "is a directory";
    }

    if (!is_valid) {
        printf(
            "%s[ERROR] Buildfile:%s %s'%s' %s!%s\n",
            output_styles.red_bold,
            output_styles.initial,
            output_styles.red,
            buildfile,
            error,
            output_styles.initial
        );
    }

    return is_valid;
}
