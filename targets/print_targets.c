#include "phingc_target.h"
#include "print_target.h"
#include "print_targets.h"

void print_targets(PhingCTarget **targets, const int targets_count, const int longest_target_name_length) {
    for (int i = 0; i < targets_count; i++) {
        print_target(targets[i], longest_target_name_length);
    }
}
