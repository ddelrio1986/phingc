#include <string.h>
#include "get_max_target_name_length.h"

int get_max_target_name_length(PhingCTarget **targets, const int targets_count) {
    size_t max_length = 0;
    for (int i = 0; i < targets_count; i++) {
        const size_t name_length = strlen(targets[i]->name);
        if (name_length > max_length) {
            max_length = name_length;
        }
    }
    return (int) max_length;
}
