#include <string.h>
#include "is_debug_enabled.h"

bool is_debug_enabled(const int argc, char *argv[]) {
    bool debug_enabled = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "-debug") == 0) {
            debug_enabled = true;
            break;
        }
    }
    return debug_enabled;
}
