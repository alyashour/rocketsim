# Architecture
Dec. 9, 2025.

The system has 3 parts. I've decided all of them will sit on separate processes for isolation and also to work on my systems programming.

The three parts are:
1. The controller. This takes in the rocket's current situation and figures out the control outputs in order for it to work.
2. The simulation. This calculates the physics (gravitational, collision, aerodynamic, etc.).
3. The Visualizer. This is the visualization of the system.

## Launching

Each process has it's own main file. The visualizer launches all three like so:
```c
#include <unistd.h>     // posix headers
#include <sys/types.h>  // pid_t, size_t, etc.
#include <sys/wait.h>   // wait()

int main() {
    pid_t pids[3];
    const char* programs[] = { "simulator", "controller", "visualizer" };

    // spawn child processes
    for (int i = 0; i < 3; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            return 1;
        }

        if (pid == 0) { // child process
            // move to new process
            // execl(path, prgrm name, arg1, arg2, arg..., NULL);
            execl(programs[i], programs[i], NULL); // never returns if successful
            perror("execl failed");
            return 1;
        } else if (pid > 0) { // parent process
            pids[i] = pid;
            printf("Launched %s with PID %d\n", programs[i], pid);
        }
    }

    // wait for children to exit
    for (int i = 0; i < 3 ; ++i) {
        int status;
        waitpid(pids[i], &status, 0);
        printf("%s exited with status %d\n", progrems[i], status);
    }

    return 0;
}
```

## IPC

These modules will talk to eachother through double buffered shared memory.
```c
typedef struct {
    ...
} State;

typedef struct {
    atomic_int current;
    State buffers[2];
} DoubleBuffer;

DoubleBuffer* create_shared_memory() {
    int fd = shm_open("/path", O_CREAT | O_RDWR, 0666);
    if (fd < 0) { perror("shm_open failed"); exit(1); }

    if (ftruncate(fd, sizeof(DoubleBuffer)) < 0) { perror("fdtruncate"); exit(1); }
    
    DoubleBuffer *db = mmap(NULL, sizeof(DoubleBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (db == MAP_FAILED) { perror("mmap"); exit(1); }

    atomic_store(&db->current, 0); // start with buffer as 0
    return db;
}
```
