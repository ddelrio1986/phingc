#include <stdlib.h>
#include "phingc_target.h"

void phingc_target_free(const PhingCTarget *target) {
    free(target->name);
    free(target->description);
    free(target->depends);
}

void phingc_targets_free(PhingCTarget **targets, const int targets_count) {
    for (int i = 0; i < targets_count; i++) {
        phingc_target_free(targets[i]);
        free(targets[i]);
    }
}

PhingCTarget phingc_target_new(void) {
    return (PhingCTarget){nullptr, nullptr, nullptr};
}
