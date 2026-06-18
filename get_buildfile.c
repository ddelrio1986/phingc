#include <stdio.h>
#include <string.h>
#include "get_buildfile.h"
#include "output_styles.h"

const char *get_buildfile(const int argc, char *argv[]) {
    auto buildfile = "build.xml";
    for (int i = 1; i < argc; i++) {
        // buildfile (-f|-buildfile <file>)
        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "-buildfile") == 0) {
            if (i + 1 >= argc) {
                printf(
                    "%s[ERROR]%s %sYou must specify a buildfile when using the -f or -buildfile argument.%s\n",
                    output_styles.red_bold,
                    output_styles.initial,
                    output_styles.red,
                    output_styles.initial
                );
                return nullptr;
            }
            buildfile = argv[++i];
        }
    }

    return buildfile;
}
