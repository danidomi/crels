#include "release.h"

// Enumerated types for OS and Arch
typedef enum {
    Darwin,
    Windows,
    Linux,
    UnknownOS
} OS;

typedef enum {
    x86_64,
    armv7l,
    aarch64,
    UnknownArch
} Arch;

typedef struct {
    const char *name;
    OS os;
    const char *archName;
    Arch arch;
    char *dockerRepo;
} releaseMapping;

releaseMapping releaseMappings[] = {
        {"Linux", Linux, "x86_64",  x86_64,  "amd64/debian"},
        {"Linux", Linux, "i386",    x86_64,  "i386/debian"},
        {"Linux", Linux, "armv7l",  armv7l,  "arm32v7/debian"},
        {"Linux", Linux, "aarch64", aarch64, "arm64v8/debian"},
        // Add more mappings as needed
};

// Function to convert string to OS enum
char *str_2_docker_repo(const char *os, const char *arch) {
    for (size_t i = 0; i < sizeof(releaseMappings) / sizeof(releaseMappings[0]); i++) {
        if (strcmp(os, releaseMappings[i].name) == 0 && strcmp(arch, releaseMappings[i].archName) == 0) {
            return releaseMappings[i].dockerRepo;
        }
    }

    return NULL;
}

int docker_release(const char *dockerRepo) {
    char command[256];
    snprintf(command, sizeof(command),
             "docker run -it -v .:/app -w /app %s:latest bash -c \"apt-get update && apt-get install -y zip make gcc && make clean release\"",
             dockerRepo);
    return system(command);
}

int handle_single_release(const char *release_type) {
    // Check if the package includes a ' ' character
    if (strchr(release_type, ' ') == NULL) {
        printf("%s: '%s release <os> <arch>'\n", CMD, CMD);
        return 1;
    }

    char *osString = NULL;
    char *archString = NULL;
    char *dockerRepo = NULL;
    char *string = strdup(release_type);

    osString = strsep(&string, " ");
    archString = strsep(&string, "\0");  // Change "\n" to "\0"

    dockerRepo = str_2_docker_repo(osString, archString);

    if (docker_release(dockerRepo) != 0) {
        return 1;
    }
    // Free the allocated memory for 'string'
    free(string);

    return 0;
}

int handle_file_release() {
    FILE *file = fopen(CRELS_FILE, "r");

    if (!file) {
        printf("Error opening %s file", CRELS_FILE);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    char *osString = NULL;
    char *archString = NULL;
    char *dockerRepo = NULL;

    while (fgets(line, sizeof(line), file)) {
        // Check if the line contains at least one space
        if (strchr(line, ' ') == NULL) {
            printf("invalid release: %s\n", line);
            continue;
        }

        if (line[0] == '#' || strlen(line) == 0) {
            continue;  // Skip comments
        }

        char *string = strdup(line);
        osString = strsep(&string, " \n");
        archString = strsep(&string, " \n");
        dockerRepo = str_2_docker_repo(osString, archString);
        if (docker_release(dockerRepo) != 0) {
            printf("error");
            return 1;
        }
    }

    return fclose(file);
}

int create_folder() {
    // Use the stat function to check if the folder exists
    struct stat st;
    if (stat(FOLDER, &st) != 0) {
        // The folder doesn't exist, so create it
        return mkdir(FOLDER, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    return 0;
}


int release(const char *release_type) {
    int success = create_folder();
    if (success != 0) {
        printf("unable to create '%s' folder", FOLDER);
        return success;
    }

    if (release_type == NULL) {
        return handle_file_release();
    }

    return handle_single_release(release_type);
}


