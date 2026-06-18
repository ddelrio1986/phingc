#include <stdlib.h>
#include "phingc_target.h"

void phingc_target_free(const PhingCTarget *target) {
    free(target->name);
    free(target->description);
    free(target->depends);
}

PhingCTarget phingc_target_new(void) {
    return (PhingCTarget){nullptr, nullptr, nullptr};
}
