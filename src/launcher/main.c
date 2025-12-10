/**
 * Starts up RocketSim's subcomponents.
 * ONLY SUPPORTS LINUX.
 * get_parent_dir depends on linux features.
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_CHILDREN 3

/**
 * Get's the parent dir of this executable. Saves it to path.
 */
ssize_t get_parent_dir(char* const path, const size_t path_len) {
    // get executable location
    ssize_t len = readlink("/proc/self/exe", path, path_len);

    // check err
    if (len == -1)
        return len;

    // term with 0
    path[len] = '\0';

    // get parent dir
    char* last_slash = strrchr(path, '/');
    if (last_slash) {
        *last_slash = '\0'; // truncate early
    }

    return last_slash ? last_slash - path : len;
}

int main() {
    // get the bin's directory
    char exec_dir_path[PATH_MAX]; // we will add 10 chars later so must be less than PATH_MAX - 10
    if (get_parent_dir(exec_dir_path, PATH_MAX) == -1) {
        perror("readlink");
        fprintf(stderr, "Failed to get path\n");
        return -1;
    }

    // all the child processes
    pid_t pids[NUM_CHILDREN];
    const char* programs[NUM_CHILDREN] = {"simulator", "controller", "visualizer"};
    char program_paths[NUM_CHILDREN][PATH_MAX] = {};
    for (int i = 0; i < NUM_CHILDREN; ++i) {
        int written = snprintf(program_paths[i], PATH_MAX, "%s/%s", exec_dir_path, programs[i]);
        if (written >= PATH_MAX) {
            fprintf(stderr, "Error: path too long for %s\n", programs[i]);
        }
    }

    // start them
    for (int i = 0; i < NUM_CHILDREN; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            return 1;
        }

        if (pid == 0) { // child process
            // move to new process
            execl(program_paths[i], programs[i], (char*)NULL);
            perror("execl failed");
            exit(1);
        } else if (pid > 0) { // parent process
            pids[i] = pid;
            printf("Launched %s with PID %d\n", programs[i], pid);
        }
    }

    // wait for children to exit
    for (int i = 0; i < NUM_CHILDREN; ++i) {
        int status;
        waitpid(pids[i], &status, 0);
        printf("%s exited with status %d\n", programs[i], status);
    }

    return 0;
}
