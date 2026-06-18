// ReSharper disable once CppUnusedIncludeDirective
#include <stdlib.h>
#include <string.h>
#include <libxml/tree.h>
#include "get_targets.h"
#include "phingc_target.h"

PhingCTarget **get_targets(const xmlNode *root_node, int *targets_count) {
    *targets_count = 0;
    int targets_capacity = 8;
    
    PhingCTarget **targets = malloc(sizeof(PhingCTarget *) * targets_capacity);
    if (targets == nullptr) {
        return nullptr;
    }

    for (
        const xmlNode *node = root_node->children;
        node != nullptr;
        node = node->next
    ) {
        if (
            node->type == XML_ELEMENT_NODE
            && xmlStrcmp(node->name, BAD_CAST "target") == 0
        ) {
            xmlChar *target_name_prop = xmlGetProp(node, BAD_CAST "name");
            if (target_name_prop == nullptr) {
                continue;
            }

            PhingCTarget *target = malloc(sizeof(PhingCTarget));
            if (target == nullptr) {
                xmlFree(target_name_prop);
                phingc_targets_free(targets, *targets_count);
                free(targets);
                *targets_count = 0;
                return nullptr;
            }
            *target = phingc_target_new();

            target->name = strdup((char *) target_name_prop);
            xmlFree(target_name_prop);

            xmlChar *target_description_prop = xmlGetProp(node, BAD_CAST "description");
            if (target_description_prop != nullptr) {
                target->description = strdup((char *) target_description_prop);
            }
            xmlFree(target_description_prop);

            xmlChar *target_depends_prop = xmlGetProp(node, BAD_CAST "depends");
            if (target_depends_prop != nullptr) {
                target->depends = strdup((char *) target_depends_prop);
            }
            xmlFree(target_depends_prop);

            if (*targets_count == targets_capacity) {
                targets_capacity *= 2;
                PhingCTarget **new_targets = realloc(targets, targets_capacity * sizeof(PhingCTarget *));
                if (new_targets == nullptr) {
                    phingc_target_free(target);
                    free(target);
                    phingc_targets_free(targets, *targets_count);
                    free(targets);
                    *targets_count = 0;
                    return nullptr;
                }
                targets = new_targets;
            }
            targets[*targets_count] = target;
            (*targets_count)++;
        }
    }

    return targets;
}
