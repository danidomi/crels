#include "crels.h"

int execute(int argc, char *argv[]) {
    if (argc < 2) {
        return usage();
    }

    if (strcmp("version", argv[1]) == 0) {
        return version();
    } else if (strcmp("release", argv[1]) == 0) {
        if (argc == 2) {
            return release(NULL);
        } else if (argc == 4) {
            return release(argv[2]);
        } else {
            return usage();
        }
    } else if (strcmp("clean", argv[1]) == 0) {
        return clean();
    } else {
        return usage();
    }
}