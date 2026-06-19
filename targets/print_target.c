#include <stdio.h>
#include "../output_styles.h"
#include "print_target.h"

void print_target(const PhingCTarget *target, const int target_column_length) {
    // Print out the target's name. We use target_column_length to specify the width to reserve for the target name
    // column. This length is calculated from the longest target name in the buildfile.
    printf(
        " %s%-*s%s",
        output_styles.purple,
        target_column_length,
        target->name,
        output_styles.initial
    );

    // Print the target's description if there is one.
    bool has_description = false;
    if (target->description != nullptr) {
        has_description = true;
        printf(" %s%s%s\n", output_styles.purple, target->description, output_styles.initial);
    }

    // Print the target's "depends" if there is one.
    bool has_depends = false;
    if (target->depends != nullptr) {
        has_depends = true;

        // If a description was printed then we're on a new line. We need to print a bunch of space so we line up our
        // output with the description that was printed before.
        if (has_description) {
            printf("%-*s ", target_column_length, "");
        }

        printf(
            " %s- depends on: %s%s\n",
            output_styles.purple,
            target->depends,
            output_styles.initial
        );
    }

    if (!has_description && !has_depends) {
        putchar('\n');
    }
}