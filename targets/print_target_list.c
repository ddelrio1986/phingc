#include <stdio.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <stdlib.h>
#include "../output_styles.h"
#include "get_default_target.h"
#include "get_max_target_name_length.h"
#include "get_targets.h"
#include "phingc_target.h"
#include "print_target_list.h"
#include "print_targets.h"

bool print_target_list(const char *buildfile, const xmlNode *buildfile_root_node) {
    // Get the full path the buildfile.
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

    // Get the length of the longest target name.
    const int longest_target_name_length = get_max_target_name_length(targets, targets_count);

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

    PhingCTarget **main_targets = malloc(sizeof(PhingCTarget *) * targets_count);
    if (main_targets == nullptr) {
        printf(
            "%s[ERROR]%s %sUnable to allocate memory for main targets.%s\n",
            output_styles.red_bold,
            output_styles.initial,
            output_styles.red,
            output_styles.initial
        );
        return false;
    }
    PhingCTarget **subtargets = malloc(sizeof(PhingCTarget *) * targets_count);
    if (subtargets == nullptr) {
        printf(
            "%s[ERROR]%s %sUnable to allocate memory for subtargets.%s\n",
            output_styles.red_bold,
            output_styles.initial,
            output_styles.red,
            output_styles.initial
        );
        return false;
    }
    int main_targets_count = 0;
    int subtargets_count = 0;
    for (int i = 0; i < targets_count; i++) {
        if (targets[i]->description != nullptr) {
            main_targets[main_targets_count++] = targets[i];
        } else {
            subtargets[subtargets_count++] = targets[i];
        }
    }

    printf("\n%sMain targets:\n", output_styles.purple_bold);
    for (int i = 0; i < 80; i++) {
        putchar('-');
    }
    printf("%s\n", output_styles.initial);

    print_targets(main_targets, main_targets_count, longest_target_name_length);
    putchar('\n');

    printf("%sSubtargets:\n", output_styles.purple_bold);
    for (int i = 0; i < 80; i++) {
        putchar('-');
    }
    printf("%s\n", output_styles.initial);

    print_targets(subtargets, subtargets_count, longest_target_name_length);

    free(main_targets);
    free(subtargets);
    phingc_targets_free(targets, targets_count);
    free(targets);

    return true;
}
