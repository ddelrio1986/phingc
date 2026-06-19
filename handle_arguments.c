#include "targets/print_target_list.h"

#include "default_buildfile.h"
#include "get_buildfile.h"
#include "handle_arguments.h"
#include "is_buildfile_valid.h"
#include "is_debug_enabled.h"
#include "output_styles.h"
#include "print_argument_debug.h"
#include "print_help.h"
#include "version.h"

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

bool handle_arguments(const int argc, char *argv[]) {
    const bool debug_enabled = is_debug_enabled(argc, argv);
    if (debug_enabled) {
        print_argument_debug(argc, argv);
    }

    bool handle_init = false;
    const char *init_buildfile_path = "build.xml";
    bool show_help = false;
    bool show_target_list = false;
    bool show_version = false;
    for (int i = 1; i < argc; i++) {
        // init (-i|-init)
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "-init") == 0) {
            handle_init = true;
            if (argc > i + 1) {
                init_buildfile_path = argv[i + 1];
            }
            break;
        }

        // help (-h|-help)
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0) {
            show_help = true;
            break;
        }

        // version (-v|-version)
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "-version") == 0) {
            show_version = true;
            break;
        }

        // list (-l|-list)
        if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "-list") == 0) {
            show_target_list = true;
        }
    }

    if (show_help) {
        print_help();
        return true;
    }

    if (show_version) {
        printf("PhingC %s\n", PHINGC_VERSION);
        return true;
    }

    if (handle_init) {
        // Check if the file already exists and bail if it does.
        FILE *init_buildfile_check = fopen(init_buildfile_path, "r");
        if (init_buildfile_check != nullptr) {
            printf(
                "%s[ERROR] Buildfile:%s %s`%s` already exists%s\n",
                output_styles.red_bold,
                output_styles.initial,
                output_styles.red,
                init_buildfile_path,
                output_styles.initial
            );
            fclose(init_buildfile_check);
            return false;
        }

        FILE *init_buildfile = fopen(init_buildfile_path, "w");
        if (init_buildfile == nullptr) {
            printf(
                "%s[ERROR] Buildfile:%s %s`%s` can not be opened for writing%s\n",
                output_styles.red_bold,
                output_styles.initial,
                output_styles.red,
                init_buildfile_path,
                output_styles.initial
            );
            return false;
        }

        if (fputs(default_buildfile, init_buildfile) == EOF) {
            printf(
                "%s[ERROR]%s %sunable to write to `%s`%s\n",
                output_styles.red_bold,
                output_styles.initial,
                output_styles.red,
                init_buildfile_path,
                output_styles.initial
            );
            fclose(init_buildfile);
            return false;
        }

        fclose(init_buildfile);
        return true;
    }

    const char *buildfile = get_buildfile(argc, argv);
    if (buildfile == nullptr) {
        return false;
    }
    if (!is_buildfile_valid(buildfile)) {
        return false;
    }

    xmlDoc *buildfile_xmldoc = xmlReadFile(buildfile, nullptr, 0);
    if (buildfile_xmldoc == nullptr) {
        printf(
            "%s[ERROR] Buildfile:%s %sXML failed to parse (not well-formed)%s\n",
            output_styles.red_bold,
            output_styles.initial,
            output_styles.red,
            output_styles.initial
        );
        xmlCleanupParser();
        return false;
    }
    const xmlNode *buildfile_root_node = xmlDocGetRootElement(buildfile_xmldoc);
    if (buildfile_root_node == nullptr) {
        printf(
            "%s[ERROR] Buildfile:%s %sbuildfile is empty%s\n",
            output_styles.red_bold,
            output_styles.initial,
            output_styles.red,
            output_styles.initial
        );
        xmlFreeDoc(buildfile_xmldoc);
        xmlCleanupParser();
        return false;
    }

    if (show_target_list) {
        if (!print_target_list(buildfile, buildfile_root_node)) {
            xmlFreeDoc(buildfile_xmldoc);
            xmlCleanupParser();
            return false;
        }
    }

    xmlFreeDoc(buildfile_xmldoc);
    xmlCleanupParser();
    return true;
}
