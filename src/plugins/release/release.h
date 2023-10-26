#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../../common/common.h"

#define MAX_LINE_LENGTH 256
#define CRELS_FILE "c.rels"

int release(const char* releaseType);