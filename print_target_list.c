#include <stdio.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <stdlib.h>
#include <string.h>
#include "targets/get_default_target.h"
#include "targets/get_targets.h"
#include "output_styles.h"
#include "phingc_target.h"
#include "print_target_list.h"


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

    // Build an array of targets.
    int targets_count;
    PhingCTarget **targets = get_targets(buildfile_root_node, &targets_count);
    if (targets == nullptr) {
        printf(
            "%s[ERROR]%s %sFailed to allocate memory for targets.%s\n",
            output_styles.red_bold,
            output_styles.initial,
            output_styles.red,
            output_styles.initial
        );
        xmlFree(project_default_prop);
        return false;
    }

    // Get the default target.
    const PhingCTarget *default_target = get_default_target((char *) project_default_prop, targets, targets_count);
    xmlFree(project_default_prop);

    if (default_target != nullptr) {
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

    // Free the array of targets.
    for (int i = 0; i < targets_count; i++) {
        phingc_target_free(targets[i]);
        free(targets[i]);
    }
    free(targets);

    printf("\n%sMain targets:\n", output_styles.purple_bold);
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
