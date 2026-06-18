#include <stdio.h>
#include "output_styles.h"
#include "print_help.h"

void print_help() {
    printf("%sphingc [options] [target [target2 [target3] ...]]%s\n", output_styles.bold, output_styles.initial);
    printf("%sOptions:%s\n", output_styles.bold, output_styles.initial);
    printf(" %-21sprint this message\n", "-h -help");
    printf(" %-21slist available targets in this project\n", "-l -list");
    printf(" %-21sprint the version information and exit\n", "-v -version");
    printf(" %-21sprint debugging information\n", "-d -debug");
    printf(" %-21suse given buildfile\n", "-f -buildfile <file>");
}
