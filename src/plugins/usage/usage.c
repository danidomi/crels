#include "usage.h"

int usage() {
    printf("%s is a tool for managing C source code.\n"
           "\n"
           "Usage:\n"
           "\n"
           "\t%s <cmd> [arguments]\n"
           "\n"
           "The commands are:\n"
           "\n"
           "\tclean       remove object files and cached files\n"
           "\trelease     compile and release dependencies\n"
           "\tversion     print %s version\n", CMD, CMD, CMD);
    return 0;
}