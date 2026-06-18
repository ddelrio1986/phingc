// TODO: for -f support scanning upwards for build.xml
// TODO: wire up -q and -S

#include <stdlib.h>
#include "handle_arguments.h"

int main(const int argc, char *argv[]) {
    if (!handle_arguments(argc, argv)) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
