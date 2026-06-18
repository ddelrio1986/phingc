#ifndef PHINGC_TARGETS_GET_TARGETS_H
#define PHINGC_TARGETS_GET_TARGETS_H
#include <libxml/tree.h>
#include "phingc_target.h"

PhingCTarget **get_targets(const xmlNode *root_node, int *targets_count);

#endif //PHINGC_TARGETS_GET_TARGETS_H
