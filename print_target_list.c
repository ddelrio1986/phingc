#include <stdio.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <stdlib.h>
#include <string.h>
#include "output_styles.h"
#include "phingc_target.h"
#include "print_target_list.h"

// TODO: finish -l output with alphabetize

bool print_target_list(const char *buildfile, const xmlNode *buildfile_root_node) {
    char *resolved_buildfile = realpath(buildfile, nullptr);
    if (resolved_buildfile == nullptr) {
        printf(
            "%s[ERROR] Buildfile:%s %s'%s' does not exist!%s\n",
            output_styles.red_bold,
            output_styles.initial,
            output_styles.red,
            buildfile,
            output_styles.initial
        );
        return false;
    }

    printf(
        "%sBuildfile:%s %s%s%s\n",
        output_styles.cyan_bold,
        output_styles.initial,
        output_styles.cyan,
        resolved_buildfile,
        output_styles.initial
    );
    free(resolved_buildfile);

    // Check if first tag is project.
    if (xmlStrcmp(buildfile_root_node->name, BAD_CAST "project") != 0) {
        printf(
            "%s[ERROR]%s %sUnexpected tag <%s> in top-level of build file.%s\n",
            output_styles.red_bold,
            output_styles.initial,
            output_styles.red,
            (char *) buildfile_root_node->name,
            output_styles.initial
        );
        return false;
    }

    // Check if the default attribute is on the project tag.
    xmlChar *project_default_prop = xmlGetProp(buildfile_root_node, BAD_CAST "default");
    if (project_default_prop == nullptr) {
        printf(
            "%s[ERROR]%s %sThe default attribute of project is required.%s\n",
            output_styles.red_bold,
            output_styles.initial,
            output_styles.red,
            output_styles.initial
        );
        return false;
    }

    xmlChar *project_description_prop = xmlGetProp(buildfile_root_node, BAD_CAST "description");
    if (project_description_prop != nullptr) {
        printf(
            "%s%s%s\n",
            output_styles.cyan,
            (char *) project_description_prop,
            output_styles.initial
        );
        xmlFree(project_description_prop);
    }

    printf("%sDefault target:\n", output_styles.purple_bold);
    for (int i = 0; i < 80; i++) {
        putchar('-');
    }
    printf("%s\n", output_styles.initial);

    // Get longest target name.
    int longest_target_name_length = 0;
    for (const xmlNode *node = buildfile_root_node->children; node != nullptr; node = node->next) {
        if (node->type == XML_ELEMENT_NODE && xmlStrcmp(node->name, BAD_CAST "target") == 0) {
            xmlChar *name = xmlGetProp(node, BAD_CAST "name");
            if (name == nullptr) {
                continue;
            }
            const int len = xmlStrlen(name);
            if (len > longest_target_name_length) longest_target_name_length = len;
            xmlFree(name);
        }
    }

    // Build a pointer to an array of pointers to PhingCTarget structs.
    int targets_count = 0;
    int targets_capacity = 8;
    // ReSharper disable once CppDFAMemoryLeak
    PhingCTarget **targets = malloc(sizeof(PhingCTarget *) * targets_capacity);
    for (
        const xmlNode *a_buildfile_node = buildfile_root_node->children;
        a_buildfile_node != nullptr;
        a_buildfile_node = a_buildfile_node->next
    ) {
        if (
            a_buildfile_node->type == XML_ELEMENT_NODE
            && xmlStrcmp(a_buildfile_node->name, BAD_CAST "target") == 0
        ) {
            xmlChar *target_name_prop = xmlGetProp(a_buildfile_node, BAD_CAST "name");
            if (target_name_prop == nullptr) {
                continue;
            }

            PhingCTarget *target = malloc(sizeof(PhingCTarget));
            *target = phingc_target_new();

            target->name = strdup((char *) target_name_prop);
            xmlFree(target_name_prop);

            xmlChar *target_description_prop = xmlGetProp(a_buildfile_node, BAD_CAST "description");
            if (target_description_prop != nullptr) {
                target->description = strdup((char *) target_description_prop);
            }
            xmlFree(target_description_prop);

            xmlChar *target_depends_prop = xmlGetProp(a_buildfile_node, BAD_CAST "depends");
            if (target_depends_prop != nullptr) {
                target->depends = strdup((char *) target_depends_prop);
            }
            xmlFree(target_depends_prop);

            if (targets_count == targets_capacity) {
                targets_capacity *= 2;
                // ReSharper disable once CppDFAMemoryLeak
                PhingCTarget **new_targets = realloc(targets, targets_capacity * sizeof(PhingCTarget *));
                if (new_targets == nullptr) {
                    for (int i = 0; i < targets_count; i++) {
                        phingc_target_free(targets[i]);
                        free(targets[i]);
                    }
                    free(targets);
                    phingc_target_free(target);
                    free(target);
                    xmlFree(project_default_prop);
                    // ReSharper disable once CppDFAMemoryLeak
                    return false;
                }
                targets = new_targets;
            }
            targets[targets_count] = target;
            targets_count++;
        }
    }
    for (int i = 0; i < targets_count; i++) {
        phingc_target_free(targets[i]);
    }
    free(targets);

    // Build a PhingCTarget populated with the node's attributes.
    PhingCTarget *default_target = malloc(sizeof(PhingCTarget));
    *default_target = phingc_target_new();
    for (
        const xmlNode *a_buildfile_node = buildfile_root_node->children;
        a_buildfile_node != nullptr;
        a_buildfile_node = a_buildfile_node->next
    ) {
        if (
            a_buildfile_node->type == XML_ELEMENT_NODE
            && xmlStrcmp(a_buildfile_node->name, BAD_CAST "target") == 0
        ) {
            // Grab the "name" property of the current XML node.
            xmlChar *a_target_name_prop = xmlGetProp(a_buildfile_node, BAD_CAST "name");
            if (a_target_name_prop == nullptr) {
                continue;
            }

            // Check if the value of the "name" property on the current XML node matches the name of the default
            // target for the project.
            if (xmlStrcmp(a_target_name_prop, project_default_prop) == 0) {
                // Within this if-statement we populate a PhingCTarget with the attributes of the current target node.
                default_target->name = strdup((char *) a_target_name_prop);

                xmlChar *a_target_description_prop = xmlGetProp(a_buildfile_node, BAD_CAST "description");
                if (a_target_description_prop != nullptr) {
                    default_target->description = strdup((char *) a_target_description_prop);
                }
                xmlFree(a_target_description_prop);

                xmlChar *a_target_depends_prop = xmlGetProp(a_buildfile_node, BAD_CAST "depends");
                if (a_target_depends_prop != nullptr) {
                    default_target->depends = strdup((char *) a_target_depends_prop);
                }
                xmlFree(a_target_depends_prop);

                xmlFree(a_target_name_prop);
                break;
            }

            xmlFree(a_target_name_prop);
        }
    }

    if (default_target->name != nullptr) {
        // Print the name of the default target.
        printf(
            " %s%-*s%s",
            output_styles.purple,
            longest_target_name_length,
            default_target->name,
            output_styles.initial
        );

        // Print the "depends" of the default target if it has one.
        if (default_target->depends != nullptr) {
            printf(
                " %s- depends on: %s%s",
                output_styles.purple,
                default_target->depends,
                output_styles.initial
            );
        }

        putchar('\n');
    }
    phingc_target_free(default_target);
    free(default_target);
    xmlFree(project_default_prop);
    putchar('\n');

    printf("%sMain targets:\n", output_styles.purple_bold);
    for (int i = 0; i < 80; i++) {
        putchar('-');
    }
    printf("%s\n", output_styles.initial);

    printf("%sSubtargets:\n", output_styles.purple_bold);
    for (int i = 0; i < 80; i++) {
        putchar('-');
    }
    printf("%s\n", output_styles.initial);

    for (const xmlNode *node = buildfile_root_node->children; node != nullptr; node = node->next) {
        if (node->type == XML_ELEMENT_NODE && xmlStrcmp(node->name, BAD_CAST "target") == 0) {
            xmlChar *name = xmlGetProp(node, BAD_CAST "name");
            if (name == nullptr) {
                continue;
            }
            printf(
                " %s%-*s - TODO: Alpabetize and list depends on.%s\n",
                output_styles.purple,
                longest_target_name_length,
                (char *) name,
                output_styles.initial
            );
            xmlFree(name);
        }
    }

    return true;
}
