#include "output_styles.h"
#include "print_buildfile.h"

#include <stdlib.h>
#include <stdio.h>

bool print_buildfile(const char *buildfile) {
    char *buildfile_realpath = realpath(buildfile, nullptr);
    if (buildfile_realpath == nullptr) {
        printf(
            "%s[ERROR] Buildfile:%s %s'%s' does not exist!%s\n",
            output_styles.red_bold,
            output_styles.initial,
            output_styles.red,
            buildfile,
            output_styles.initial
        );
        return false;
    }

    printf(
        "%sBuildfile:%s %s%s%s\n",
        output_styles.cyan_bold,
        output_styles.initial,
        output_styles.cyan,
        buildfile_realpath,
        output_styles.initial
    );
    free(buildfile_realpath);
    return true;
}
