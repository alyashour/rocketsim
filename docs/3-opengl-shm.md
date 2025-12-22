# OpenGL & SHM
Dec. 19, 2025.

## Hello Triangle

After wrangling GLFW and GLAD to work on my computer + some boiler plate to open a window it's time to start work on some specific functionality of the simulation.

My renderer right now does a whole lot of nothing and my simulation does even less. Let's start by having the renderer draw a triangle to the screen, the color and position of which is decided by the simulation and communicated through shared memory.

## OpenGL

Now for the fun part. My OpenGL is somewhat rusty. I'll be following the wonderful https://learnopengl.com/ guide to refresh.
A quick crash course on coding without OOP and a C triangle module later I have: 
```c
typedef struct TriangleObj TriangleObj;
TriangleObj* createTriangle();
void drawTriangle(const TriangleObj* const t);
void destroyTriangle(TriangleObj* const t);
```
And 10 lines of OpenGL that draw a triangle.

## Shared Memory
First I created a test shared memory setup with just a single integer across two files:

Simulation:
```c
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
```

Visualizer:
```c
// create shared mem
printf("[VIS] Opening shm with simulation...\n");
int fd = shm_open("rocketsim", O_RDONLY | O_EXCL, 0666);
if (fd == -1) {
    printf("[VIS] shm_open failed.\n");
    return EXIT_FAILURE;
}
int32_t* ptr = mmap(0, sizeof(int32_t), PROT_READ, MAP_SHARED, fd, 0);
if (ptr == MAP_FAILED) {
    printf("[VIS] mmap failed.\n");
    fprintf(stderr, "error: %s\n", strerror(errno));
    return EXIT_FAILURE;
}

// ...

while (true) {
    // print ptr
    printf("[VIS] ptr shows: %d\n", *ptr);
}
```

I then updated the shared memory to instead point to a struct.

```c
typedef struct SceneFrame {
    // Position
    float x, y, z;

    // color
    float r, g, b;

    /**
     * In the future this will contain an array
     * of objects maybe, timestamp, etc.
     */
}
```

## Maths

Because I'm a masochist (aka downright thug) we're writing the maths from scratch. I've creatively named it RocketSim math (RSM).

Some juggling with transformation matrices later and ...

## Complete!
We're done 😏

<video controls src="../media/3-motion-ipc.mov" title="Motion through IPC"></video>