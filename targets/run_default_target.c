// current folder
//#include "get_default_target.h"
//#include "get_targets.h"
//#include "phingc_target.h"
#include "run_default_target.h"

// external folders
#include "../output_styles.h"
#include "../print_buildfile.h"

// system libraries
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/tree.h>

bool run_default_target(const char *buildfile_path, const xmlNode *buildfile_root_node) {
    if (!print_buildfile(buildfile_path)) {
        return false;
    }

    // Print the project's name if there is one or the file's name if there is not.
    putchar('\n');
    char *project_name = nullptr;
    xmlChar *project_name_prop = xmlGetProp(buildfile_root_node, BAD_CAST "name");
    if (project_name_prop == nullptr) {
        // Copy buildfile_path because it is const and basename() can alter what you give it.
        char *editable_buildfile_path = strdup(buildfile_path);
        if (editable_buildfile_path != nullptr) {
            // Copy what basename returns so we can free editable_buildfile_path but hang on to the value.
            const char *buildfile_basename = basename(editable_buildfile_path);
            project_name = strdup(buildfile_basename);
            free(editable_buildfile_path);
        }
    } else {
        project_name = strdup((char *) project_name_prop);
        xmlFree(project_name_prop);
    }
    if (project_name != nullptr) {
        printf(
            "%s%s > %s",
            output_styles.green_bold,
            project_name,
            output_styles.initial
        );
        free(project_name);
    }

    // Get the name of the default target from the default attribute of the project tag.
    xmlChar *project_default_prop = xmlGetProp(buildfile_root_node, BAD_CAST "default");
    if (project_default_prop == nullptr) {
        printf(
            "%s[ERROR]%s %sThe default attribute of the project tag is required.%s\n",
            output_styles.red_bold,
            output_styles.initial,
            output_styles.red,
            output_styles.initial
        );
        return false;
    }

    // Print out the name of the default target.
    printf(
        "%s%s:%s\n",
        output_styles.green_bold,
        (char *) project_default_prop,
        output_styles.initial
    );

    // Loop through the nodes to find the default target node.
    const xmlNode *target_node = nullptr;
    for (
        const xmlNode *a_target_node = buildfile_root_node->children;
        a_target_node != nullptr;
        a_target_node = a_target_node->next
    ) {
        if (
            a_target_node->type == XML_ELEMENT_NODE
            && xmlStrcmp(a_target_node->name, BAD_CAST "target") == 0
        ) {
            xmlChar *target_name_prop = xmlGetProp(a_target_node, BAD_CAST "name");
            if (target_name_prop == nullptr) {
                continue;
            }
            if (xmlStrcmp(project_default_prop, target_name_prop) == 0) {
                target_node = a_target_node;
                xmlFree(target_name_prop);
                break;
            }
            xmlFree(target_name_prop);
        }
    }
    xmlFree(project_default_prop);

    if (target_node == nullptr) {
        printf(
            "%s[ERROR]%s %sThe default target was not found.%s\n",
            output_styles.red_bold,
            output_styles.initial,
            output_styles.red,
            output_styles.initial
        );
        return false;
    }

    // Loop through the children of the default target node.
    for (
        const xmlNode *a_task_node = target_node->children;
        a_task_node != nullptr;
        a_task_node = a_task_node->next
    ) {
        // If the child is an echo task, then handle it.
        if (
            a_task_node->type == XML_ELEMENT_NODE
            && xmlStrcmp(a_task_node->name, BAD_CAST "echo") == 0
        ) {
            // Get the msg attribute of the echo tag.
            xmlChar *msg_prop = xmlGetProp(a_task_node, BAD_CAST "msg");
            if (msg_prop == nullptr) {
                continue;
            }

            // Print the message.
            printf(
                "\n    %s[echo]%s %s%s%s\n",
                output_styles.cyan_bold,
                output_styles.initial,
                output_styles.cyan,
                (char *) msg_prop,
                output_styles.initial
            );
            xmlFree(msg_prop);
        }
    }

    return true;
}
