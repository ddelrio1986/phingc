#include <stdio.h>
#include "../output_styles.h"
#include "phingc_target.h"
#include "print_targets.h"

void print_targets(PhingCTarget **targets, const int targets_count, const int longest_target_name_length) {
    for (int i = 0; i < targets_count; i++) {
        printf(
            " %s%-*s%s",
            output_styles.purple,
            longest_target_name_length,
            targets[i]->name,
            output_styles.initial
        );

        bool has_description = false;
        if (targets[i]->description != nullptr) {
            has_description = true;
            printf(" %s%s%s\n", output_styles.purple, targets[i]->description, output_styles.initial);
        }

        bool has_depends = false;
        if (targets[i]->depends != nullptr) {
            has_depends = true;
            if (has_description) {
                printf("%-*s ", longest_target_name_length, "");
            }
            printf(
                " %s- depends on: %s%s\n",
                output_styles.purple,
                targets[i]->depends,
                output_styles.initial
            );
        }

        if (!has_description && !has_depends) {
            putchar('\n');
        }
    }
}
