#include <stdio.h>
#include "output_styles.h"
#include "print_argument_debug.h"

void print_argument_debug(const int argc, char *argv[]) {
    printf(
        "%s[DEBUG] Number of arguments:%s %s%d%s\n",
        output_styles.purple_bold,
        output_styles.initial,
        output_styles.purple,
        argc - 1,
        output_styles.initial
    );

    printf(
        "%s[DEBUG] Name of program:%s %s%s%s\n",
        output_styles.purple_bold,
        output_styles.initial,
        output_styles.purple,
        argv[0],
        output_styles.initial
    );

    printf(
        "%s[DEBUG] Arguments:%s\n",
        output_styles.purple_bold,
        output_styles.initial
    );
    for (int i = 1; i < argc; i++) {
        printf(
            "%s[DEBUG]%s \t%s• \"%s\"%s\n",
            output_styles.purple_bold,
            output_styles.initial,
            output_styles.purple,
            argv[i],
            output_styles.initial
        );
    }
}
