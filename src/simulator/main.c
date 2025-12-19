#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main() {
    printf("Hello, Simulator!\n");

    printf("[SIM] Opening shm with visualizer...\n");
    int fd = shm_open("rocketsim", O_RDWR | O_CREAT, 0644, 0);
    if (fd == -1) {
        printf("[SIM] shm_open failed.\n");
        return EXIT_FAILURE;
    }
    ftruncate(fd, sizeof(int32_t));
    int32_t* ptr = mmap(0, sizeof(int32_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        printf("[SIM] mmap failed.\n");
        fprintf(stderr, "error: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    printf("[SIM] Setting ptr to 0\n");
    *ptr = 0;
    sleep(5); // wait 5 seconds

    printf("[SIM] Setting ptr to 1\n");
    *ptr = 1;
    sleep(5); // wait 5 seconds

    printf("[SIM] Setting ptr to 2\n");
    *ptr = 2;
    sleep(5); // wait 5 seconds

    printf("[SIM] Closing...\n");

    return 0;
}
