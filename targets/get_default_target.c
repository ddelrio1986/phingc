#include <string.h>
#include "get_default_target.h"
#include "phingc_target.h"

PhingCTarget *get_default_target(const char *default_target_name, PhingCTarget **targets, int targets_count) {
    for (int i = 0; i < targets_count; i++) {
        if (strcmp(targets[i]->name, default_target_name) == 0) {
            return targets[i];
        }
    }
    return nullptr;
}
