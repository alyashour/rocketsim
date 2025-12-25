#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

#include "scene_frame.h"

#define RS_SHM_PATH "/rocketsim"

float dr = 0.01f, dg = 0.02f, db = 0.03f;
float dx = 0.01f, dy = 0.01f;

void sleepms(int time_ms) {
    static struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = time_ms * 1e6L;
    nanosleep(&ts, NULL);
}

int main() {
    printf("Hello, Simulator!\n");

    printf("[SIM] Opening shm with visualizer...\n");
    shm_unlink(RS_SHM_PATH); // unlink in case there's already one there
    int fd = shm_open(RS_SHM_PATH, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        fprintf(stderr, "[SIM] shm_open failed.\n%s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    ftruncate(fd, sizeof(int32_t));
    SceneFrame* ptr = mmap(0, sizeof(SceneFrame), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "[SIM] mmap failed.\n%s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    // set defaults
    ptr->x = 0;
    ptr->y = 0;
    ptr->z = 0;
    ptr->r = 1.0f;
    ptr->g = 0.4f;
    ptr->b = 0.0f;

    while (true) {
       // update color
       ptr->r += dr;
       ptr->g += dg;
       ptr->b += db;

       if (fabs(ptr->r) > 1.0f) dr *= -1.f;
       if (fabs(ptr->g) > 1.0f) dg *= -1.f;
       if (fabs(ptr->b) > 1.0f) db *= -1.f;

       // update position
       ptr->x += dx;
       ptr->y += dy;

       if (fabs(ptr->x) > 0.5f) dx *= -1.f;
       if (fabs(ptr->y) > 0.5f) dy *= -1.f;

       sleepms(50);
    }

    return 0;
}
