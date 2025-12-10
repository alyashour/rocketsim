#include <limits.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * Assumes path has is PATH_MAX long.
 */
ssize_t get_parent_dir(char *path) {
    ssize_t len = readlink("/proc/self", path, PATH_MAX);

    // check err
    if (len == -1)
        return len;

    // term with 0
    path[len] = '\0';

    return len;
}

int main() {
    char *path[PATH_MAX];
    get_parent_dir(path);
    printf("%s\n", path);
    return 0;
    pid_t pids[3];
    const char *programs[] = {"simulator", "controller", "visualizer"};

    for (int i = 0; i < 3; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            return 1;
        }

        if (pid == 0) { // child process
            // move to new process
            execl(programs[i], programs[i], NULL);
            perror("execl failed");
            return 1;
        } else if (pid > 0) { // parent process
            pids[i] = pid;
            printf("Launched %s with PID %d\n", programs[i], pid);
        }
    }

    // wait for children to exit
    for (int i = 0; i < 3; ++i) {
        int status;
        waitpid(pids[i], &status, 0);
        printf("%s exited with status %d\n", programs[i], status);
    }

    return 0;
}
