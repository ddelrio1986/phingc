#ifndef PHINGC_PHINGC_TARGET_H
#define PHINGC_PHINGC_TARGET_H

typedef struct {
    char *name;
    char *description;
    char *depends;
} PhingCTarget;

void phingc_target_free(const PhingCTarget *target);

PhingCTarget phingc_target_new(void);

#endif //PHINGC_PHINGC_TARGET_H
